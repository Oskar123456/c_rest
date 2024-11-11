#include "../include/incl.h"
#include "../include/weather_report.h"
#include "../include/c_log.h"
#include "../external/mlib/m-tuple.h"
#include "../external/mlib/m-array.h"
#include "../external/mlib/m-serial-json.h"
#include <curl/curl.h>
#include <curl/easy.h>

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * -----------------------
 */

void get_headlines_ISO3166_2(const char* ISO3166_2);
void get_packinglist(const char* category);
weather_report_t* get_weather(const char* city);
bool get_weather_str(bstring_t dest, const char* city);


