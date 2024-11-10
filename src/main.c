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

int main(int argc, char *argv[])
{
    c_log_init(stderr, LOG_LEVEL_SUCCESS);

    PGconn* db_connection = db_connect();
    db_print_table(db_connection, "trip");
    db_print_table(db_connection, "guide");
    PQfinish(db_connection);

    //get_packinglist("beach");
    //get_headlines_ISO3166_2("us");
    get_weather("London");

    int exit_code = 0; // serve(argc, argv);

    return exit_code;
}







































