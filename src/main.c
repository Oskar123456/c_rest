#include "../include/mongoose.h"
#include "../include/incl.h"
#include "../include/serv.h"
#include "../include/db.h"
#include "../include/c_log.h"
#include "../include/scrape.h"

#include <postgresql/libpq-fe.h>

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



    return success;
}

int main(int argc, char *argv[])
{
    c_log_init(stderr, LOG_LEVEL_SUCCESS);

    PGconn* db_connection = db_connect();

    bool sql_script_res = db_exec_script(db_connection, "resources/sql/weather_api_create.sql");

    //db_print_table(db_connection, "trip");
    //db_print_table(db_connection, "guide");

    weather_report_t *wr = get_weather("London");
    //printf("%s\n", string_get_cstr((*wr)->location->name));


    int exit_code = 0; // serve(argc, argv);

    PQfinish(db_connection);
    return exit_code;
}







































