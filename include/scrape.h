#include "../include/incl.h"
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

TUPLE_DEF2(location_info,
        (name,               string_t),
        (region,             string_t),
        (country,            string_t),
        (tz_id,              string_t),
        (localtime_epoch,    u64),
        (localtime,          string_t),
        (lon,                double),
        (lat,                double))
#define M_OPL_location_info_t() TUPLE_OPLIST(location_info, \
        STRING_OPLIST,  STRING_OPLIST, STRING_OPLIST,  STRING_OPLIST, \
        M_BASIC_OPLIST, STRING_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST)

TUPLE_DEF2(weather_condition,
        (text,               string_t),
        (icon,               string_t),
        (code,               i64))
#define M_OPL_weather_condition_t() TUPLE_OPLIST(weather_condition, \
        STRING_OPLIST, STRING_OPLIST, M_BASIC_OPLIST)

TUPLE_DEF2(weather_info,
        (last_updated_epoch, u64),
        (last_updated,       string_t),
        (temp_c,             double),
        (temp_f,             double),
        (is_day,             i8),
        (condition,          weather_condition_t),
        (wind_mph,           double),
        (wind_kph,           double),
        (wind_degree,        double),
        (wind_dir,           string_t),
        (pressure_mb,        double),
        (pressure_in,        double),
        (precip_mm,          double),
        (precip_in,          double),
        (humidity,           double),
        (cloud,              double),
        (feelslike_c,        double),
        (feelslike_f,        double),
        (windchill_c,        double),
        (windchill_f,        double),
        (heatindex_c,        double),
        (heatindex_f,        double),
        (dewpoint_c,         double),
        (dewpoint_f,         double),
        (vis_km,             double),
        (vis_miles,          double),
        (uv,                 double),
        (gust_mph,           double),
        (gust_kph,           double))
#define M_OPL_weather_info_t() TUPLE_OPLIST(weather_info, \
        M_BASIC_OPLIST, STRING_OPLIST,  M_BASIC_OPLIST, M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_OPL_weather_condition_t(),    M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST, STRING_OPLIST,  M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, M_BASIC_OPLIST, \
        M_BASIC_OPLIST, M_BASIC_OPLIST)

TUPLE_DEF2(weather_report,
        (location,           location_info_t),
        (current,            weather_info_t))
#define M_OPL_weather_report_t() TUPLE_OPLIST(weather_report, \
        M_OPL_weather_info_t(),  M_OPL_location_info_t())

void get_headlines_ISO3166_2(const char* ISO3166_2);
void get_packinglist(const char* category);
weather_report_t* get_weather(const char* city);


