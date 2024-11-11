#include "../include/weather_report.h"

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * -----------------------
 */

bool insert_weather_report(PGconn *conn, weather_report_t *wr)
{
    bool success = true;

    location_info_t *location = weather_report_get_at_location(*wr);
    weather_info_t *weather = weather_report_get_at_current(*wr);
    weather_condition_t *weather_condition = weather_info_get_at_condition(*weather);

    const char *stmt_str = "INSERT INTO public.location_info "
        "      (id,      name, region, country, tz_id, localtime_epoch, localtime_loc, lon, lat) "
        "VALUES(DEFAULT, $1,   $2,     $3,      $4,    $5,              $6,            $7,  $8)  "
        "RETURNING id";

    string_t stmt;
    string_init(stmt);
    string_set_str(stmt, stmt_str);

    string_t t_epoch_str, lon_str, lat_str;
    string_init(t_epoch_str); string_init(lat_str); string_init(lon_str);
    string_printf(t_epoch_str, "%lu", *location_info_get_at_localtime_epoch(*location));
    string_printf(lon_str, "%f", *location_info_get_at_lon(*location));
    string_printf(lat_str, "%f", *location_info_get_at_lat(*location));

    c_log_info("numbers", "%lu %f %f",
           *location_info_get_at_localtime_epoch(*location),
           *location_info_get_at_lon(*location),
           *location_info_get_at_lat(*location));

    const char **params = malloc(sizeof(void*) * 8);
    params[0] = string_get_cstr(*location_info_get_at_name(*location));
    params[1] = string_get_cstr(*location_info_get_at_region(*location));
    params[2] = string_get_cstr(*location_info_get_at_country(*location));
    params[3] = string_get_cstr(*location_info_get_at_tz_id(*location));
    params[4] = string_get_cstr(t_epoch_str);
    params[5] = string_get_cstr(*location_info_get_at_localtime(*location));
    params[6] = string_get_cstr(lon_str);
    params[7] = string_get_cstr(lat_str);

    PGresult *res = PQexecParams(conn, stmt_str, 8, NULL, params, NULL, NULL, 0); /* string return format */
    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
        c_log_error("insert_weather_report", PQerrorMessage(conn));
        c_log_error("insert_weather_report SQLSTATE", PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error("insert_weather_report check return", PQresultErrorMessage(res));
        success = false;
    }

    else {
        i64 ret_ids[PQntuples(res)];
        for (int i = 0; i < PQntuples(res); ++i) {
            ret_ids[i] = strtoll(PQgetvalue(res, i, 0), NULL, 10);
            c_log_info("insert_weather_report", "insert success (new id: %lld)", ret_ids[i]);
        }
    }

    string_clear(t_epoch_str);
    string_clear(lon_str);
    string_clear(lat_str);
    PQclear(res);

    return success;
}
