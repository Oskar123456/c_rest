#include "../include/scrape.h"
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
 * PRIVATE API
 * -----------------------
 */

static size_t rec_http_header(char *data, size_t size, size_t nmemb, void *clientp)
{
    bstring_t *rec_data = (bstring_t*)data;
    bstring_t *cur_data = (bstring_t*)clientp;
    c_log_debug(__FILE__, "[l:%d] clientp size before: %d", __LINE__, bstring_size(*cur_data));
    int len_prev = bstring_size(*cur_data);
    bstring_push_back_bytes(*cur_data, nmemb, *rec_data);
    int len_curr = bstring_size(*cur_data);
    c_log_debug(__FILE__, "[l:%d] clientp size after: %d", __LINE__, bstring_size(*cur_data));
    return len_curr - len_prev;
}

static size_t rec_http_body(char *data, size_t size, size_t nmemb, void *clientp)
{
    bstring_t *rec_data = (bstring_t*)data;
    bstring_t *cur_data = (bstring_t*)clientp;
    int len_prev = bstring_size(*cur_data);
    bstring_push_back_bytes(*cur_data, nmemb, *rec_data);
    int len_curr = bstring_size(*cur_data);
    return len_curr - len_prev;
}

weather_report_t* results_from_json_weather_report(bstring_t json)
{
    weather_report_t *res = malloc(sizeof(weather_report_t));
    weather_report_init(*res);

    m_serial_str_json_read_t stream;
    m_serial_str_json_read_init(stream, (char*) bstring_view(json, 0, bstring_size(json)));

    int status = weather_report_in_serial(*res, stream);

    if (status != M_SERIAL_OK_DONE) {
        c_log_error("results_from_json", "decoding failed: %d", status);
        return NULL;
    }

    m_serial_str_json_read_clear(stream);
    return res;
}

/*
 * PUBLIC API
 * -----------------------
 */

weather_report_t* get_weather(const char* city)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    bstring_t res_body, res_header;
    bstring_init(res_body); bstring_init(res_header);
    string_t url;
    string_init(url);
    string_printf(url, "http://api.weatherapi.com/v1/current.json?key=%s&q=%s&aqi=no",
            getenv("WEATHER_API_KEY"), city);
    c_log_info("get_weather url:", string_get_cstr(url));

    CURL* curl = curl_easy_init();
    CURLcode response_code;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");

    curl_easy_setopt(curl, CURLOPT_URL, string_get_cstr(url));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*) &res_header);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, rec_http_header);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &res_body);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rec_http_body);

    if ((response_code = curl_easy_perform(curl)))
        c_log_error("get_weather", curl_error_buffer);
    curl_slist_free_all(headers);

    FILE *output_file = fopen("out/http_resp_weather.json", "w+");
    if (output_file != NULL) {
        bstring_fwrite(output_file, res_body);
        c_log_info("get_weather body", (char*) bstring_view(res_body, 0, bstring_size(res_body)));
        c_log_info("get_weather header", (char*) bstring_view(res_header, 0, bstring_size(res_header)));
    }
    else
        c_log_error("get_weather", "fopen failed");

    weather_report_t *wr = results_from_json_weather_report(res_body);

    bstring_clear(res_header);
    bstring_clear(res_body);
    curl_easy_cleanup(curl);

    return wr;
}

bool get_weather_str(bstring_t dest, const char* city)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    bstring_t res_body, res_header;
    bstring_init(res_body); bstring_init(res_header);
    string_t url;
    string_init(url);
    string_printf(url, "http://api.weatherapi.com/v1/current.json?key=%s&q=%s&aqi=no",
            getenv("WEATHER_API_KEY"), city);
    c_log_info("get_weather url:", string_get_cstr(url));

    CURL* curl = curl_easy_init();
    CURLcode response_code;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");

    curl_easy_setopt(curl, CURLOPT_URL, string_get_cstr(url));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*) &res_header);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, rec_http_header);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &res_body);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rec_http_body);

    if ((response_code = curl_easy_perform(curl))) {
        c_log_error(__FILE__, "[l:%d] %s", __LINE__, curl_error_buffer);
        return false;
    }
    curl_slist_free_all(headers);

    //weather_report_t *wr = results_from_json_weather_report(dest);

    bstring_set(dest, res_body);

    bstring_clear(res_header);
    bstring_clear(res_body);
    curl_easy_cleanup(curl);

    c_log_success(__FILE__, "[l:%d] received %d bytes", __LINE__, bstring_size(dest));
    return true;
}
