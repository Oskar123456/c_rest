#include "../include/scrape.h"
#include "../external/mlib/m-tuple.h"
#include "../external/mlib/m-array.h"
#include "../external/mlib/m-serial-json.h"
#include <curl/curl.h>

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * -----------------------
 */

/*
 * TYPES
 * -----------------------
 */

TUPLE_DEF2(news_api_source,
        (id, string_t),
        (name, string_t))
#define M_OPL_news_api_source_t() TUPLE_OPLIST(news_api_source, \
        STRING_OPLIST, STRING_OPLIST)

TUPLE_DEF2(news_api_article,
        (source, news_api_source_t),
        (author, string_t),
        (title, string_t),
        (description, string_t),
        (url, string_t),
        (urlToImage, string_t),
        (publishedAt, string_t),
        (content, string_t))
#define M_OPL_news_api_article_t() TUPLE_OPLIST(news_api_article, \
        M_OPL_news_api_source_t(), STRING_OPLIST, STRING_OPLIST, \
        STRING_OPLIST, STRING_OPLIST, STRING_OPLIST, \
        STRING_OPLIST, STRING_OPLIST)

ARRAY_DEF(array_article, news_api_article_t)
#define M_OPL_array_article_t() ARRAY_OPLIST(array_article, M_OPL_news_api_article_t())

TUPLE_DEF2(news_api_results,
        (status, string_t),
        (totalResults, int),
        (articles, array_article_t))
#define M_OPL_news_api_results_t() TUPLE_OPLIST(news_api_results, \
        STRING_OPLIST, M_BASIC_OPLIST, M_OPL_array_article_t())

TUPLE_DEF2(buyingoption,
        (shopName, string_t),
        (shopUrl, string_t),
        (price, double))
#define M_OPL_buyingoption_t() TUPLE_OPLIST(buyingoption, \
        STRING_OPLIST, STRING_OPLIST, M_BASIC_OPLIST)

ARRAY_DEF(array_buyingoption, buyingoption_t)
#define M_OPL_array_buyingoption_t() ARRAY_OPLIST(array_buyingoption, M_OPL_buyingoption_t())

TUPLE_DEF2(packingitem,
        (name, string_t),
        (weightInGrams, int),
        (quantity, int),
        (description, string_t),
        (category, string_t),
        (createdAt, string_t),
        (updatedAt, string_t),
        (buyingOptions, array_buyingoption_t))
#define M_OPL_packingitem_t() TUPLE_OPLIST(packingitem, \
        STRING_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, \
        STRING_OPLIST, STRING_OPLIST, STRING_OPLIST, STRING_OPLIST, \
        M_OPL_array_buyingoption_t())

ARRAY_DEF(array_packingitem, packingitem_t)
#define M_OPL_array_packingitem_t() ARRAY_OPLIST(array_packingitem, M_OPL_packingitem_t())

TUPLE_DEF2(packinglist,
        (items, array_packingitem_t))
#define M_OPL_packinglist_t() TUPLE_OPLIST(packinglist, \
        M_OPL_array_packingitem_t())
/*
 */

/*
 * VARIABLE DECLARATIONS
 * -----------------------
 */

static const char* NEWS_API_KEY = "1d827a953b344080b5cf5c425e49669c";

/*
 * API
 * -----------------------
 */

packinglist_t* results_from_json_packinglist(string_t json)
{
    packinglist_t *res = malloc(sizeof(array_packingitem_t));
    packinglist_init(*res);

    m_serial_str_json_read_t stream;
    m_serial_str_json_read_init(stream, string_get_cstr(json));

    int status = packinglist_in_serial(*res, stream);

    if (status != M_SERIAL_OK_DONE) {
        c_log_error("results_from_json", "decoding failed: %d", status);
        return NULL;
    }

    string_t json_out_str;
    string_init(json_out_str);
    FILE *output_file = fopen("out/packinglist.json", "w+");
    m_serial_json_write_t outstream;
    m_serial_json_write_init(outstream, output_file);
    status = packinglist_out_serial(outstream, *res);

    m_serial_str_json_read_clear(stream);
    return res;
}

news_api_results_t* results_from_json_file(const char *path)
{
    news_api_results_t *res = malloc(sizeof(news_api_results_t));
    news_api_results_init(*res);

    FILE *f = fopen(path, "r");
    if (!f) {
        c_log_error("results_from_json_file", "file open failed");
        return NULL;
    }

    m_serial_json_read_t stream;
    m_serial_json_read_init(stream, f);

    int status = news_api_results_in_serial(*res, stream);

    if (status != M_SERIAL_OK_DONE) {
        c_log_error("results_from_json", "decoding failed: %d", status);
        return NULL;
    }

    news_api_results_out_str(stdout, *res);

    m_serial_json_read_clear(stream);
    fclose(f);
    return res;
}

news_api_results_t* results_from_json(string_t json)
{
    news_api_results_t *res = malloc(sizeof(news_api_results_t));
    news_api_results_init(*res);

    m_serial_str_json_read_t stream;
    m_serial_str_json_read_init(stream, string_get_cstr(json));

    int status = news_api_results_in_serial(*res, stream);

    if (status != M_SERIAL_OK_DONE) {
        c_log_error("results_from_json", "decoding failed: %d", status);
        return NULL;
    }

    news_api_results_out_str(stdout, *res);

    m_serial_str_json_read_clear(stream);
    return res;
}

static size_t rec_http(char *data, size_t size, size_t nmemb, void *clientp)
{
    string_t http_data;
    string_init(http_data);
    string_set_strn(http_data, data, nmemb);
    string_t *http_resp = (string_t*) clientp;
    int len_prev = string_size(*http_resp);
    c_log_debug("rec_http", "current data size: %d, recieved size: %d", len_prev, nmemb);
    string_cat(*http_resp, http_data);
    int len_curr = string_size(*http_resp);
    c_log_debug("rec_http", "new data size: %d", len_curr, nmemb);
    return len_curr - len_prev;
}

void get_headlines_ISO3166_2(const char* ISO3166_2)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    string_t response_buffer;
    string_init(response_buffer);
    string_t url;
    string_init(url);
    string_printf(url, "https://newsapi.org/v2/top-headlines?country=%s&apiKey=%s",
            ISO3166_2, NEWS_API_KEY);
    c_log_info("get_headlines_ISO3166_2 url:", string_get_cstr(url));

    CURL* curl = curl_easy_init();
    CURLcode response_code;

    curl_easy_setopt(curl, CURLOPT_URL, string_get_cstr(url));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &response_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rec_http);

    if ((response_code = curl_easy_perform(curl))) {
        c_log_error("get_headlines_ISO3166_2", curl_error_buffer);
    }

    FILE *output_file = fopen("out/http_resp.json", "w+");
    if (output_file != NULL) {
        string_out_str(output_file, response_buffer);
        c_log_info("get_headlines_ISO3166_2", "wrote %d bytes to file",
                string_size(response_buffer));
    }
    else
        c_log_error("get_headlines_ISO3166_2", "fopen failed");

    news_api_results_t *headlines = results_from_json_file("out/http_resp.json");

    string_clear(response_buffer);
    curl_easy_cleanup(curl);
}

void get_packinglist(const char* category)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    string_t response_buffer;
    string_init(response_buffer);
    string_t url;
    string_init(url);
    string_printf(url, "https://packingapi.cphbusinessapps.dk/packinglist/%s", category);
    c_log_info("get_packinglist url:", string_get_cstr(url));

    CURL* curl = curl_easy_init();
    CURLcode response_code;

    curl_easy_setopt(curl, CURLOPT_URL, string_get_cstr(url));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &response_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rec_http);

    if ((response_code = curl_easy_perform(curl))) {
        c_log_error("get_packinglist", curl_error_buffer);
    }

    FILE *output_file = fopen("out/http_resp_packing.json", "w+");
    if (output_file != NULL) {
        string_out_str(output_file, response_buffer);
        c_log_info("get_packinglist", "wrote %d bytes to file",
                string_size(response_buffer));
    }
    else
        c_log_error("get_packinglist", "fopen failed");

    packinglist_t *pl = results_from_json_packinglist(response_buffer);

    string_clear(response_buffer);
    curl_easy_cleanup(curl);
}

void get_weather(const char* city)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    string_t response_buffer, url;
    string_init(response_buffer); string_init(url);
    string_printf(url, "http://api.weatherapi.com/v1/current.json?key=%s&q=%s&aqi=no",
            getenv("WEATHER_API_KEY"), city);
    c_log_info("get_weather url:", string_get_cstr(url));

    CURL* curl = curl_easy_init();
    CURLcode response_code;

    curl_easy_setopt(curl, CURLOPT_URL, string_get_cstr(url));
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &response_buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rec_http);

    if ((response_code = curl_easy_perform(curl))) {
        c_log_error("get_weather", curl_error_buffer);
    }

    FILE *output_file = fopen("out/http_resp_weather.json", "w+");
    if (output_file != NULL) {
        string_out_str(output_file, response_buffer);
        c_log_info("get_weather", "wrote %d bytes to file",
                string_size(response_buffer));
    }
    else
        c_log_error("get_weather", "fopen failed");

    packinglist_t *pl = results_from_json_packinglist(response_buffer);

    string_clear(response_buffer);
    curl_easy_cleanup(curl);
}
