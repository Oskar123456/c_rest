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

static const char* NEWS_API_KEY = "1d827a953b344080b5cf5c425e49669c";

static size_t rec_http(char *data, size_t size, size_t nmemb, void *clientp)
{
    bstring_t *http_resp = (bstring_t*) clientp;
    int len_prev = bstring_size(*http_resp);
    c_log_info("rec_http", "current data size: %d, recieved size: %d", len_prev, nmemb);
    bstring_push_back_bytes(*http_resp, nmemb, data);
    int len_curr = bstring_size(*http_resp);
    c_log_info("rec_http", "new data size: %d", len_curr, nmemb);
    return len_curr - len_prev;
}

void get_headlines_ISO3166_2(const char* ISO3166_2)
{
    char curl_error_buffer[CURL_ERROR_SIZE] = { 0 };
    bstring_t response_buffer;
    bstring_init(response_buffer);
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
    if (output_file != NULL)
        c_log_info("get_headlines_ISO3166_2", "wrote %d/%d bytes to file",
                bstring_fwrite(output_file, response_buffer), bstring_size(response_buffer));
    else
        c_log_error("get_headlines_ISO3166_2", "fopen failed");
    bstring_clear(response_buffer);

    curl_easy_cleanup(curl);
}

