#include "../include/mongoose.h"
#include "../include/incl.h"
#include "../include/serv.h"
#include "../include/db.h"

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

    PGconn* db_connection = db_connect();
    db_print_table(db_connection, "trip");
    db_print_table(db_connection, "guide");
    PQfinish(db_connection);

    int exit_code = serve(argc, argv);

    return exit_code;

}







































