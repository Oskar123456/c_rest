#include "../include/mongoose.h"
#include "../include/incl.h"
#include "../include/serv.h"
#include "../include/db.h"
#include "../include/c_log.h"
#include "../include/scrape.h"
#include "../include/task.h"

#include <endian.h>
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

    //PGconn* db_connection = db_connect();

    //bool sql_script_res = db_exec_script(db_connection, "resources/sql/task_schema_create.sql");
    //if (!sql_script_res)
    //    return EXIT_FAILURE;

    //task_t my_task;
    //task_new(my_task, "wash dishes", "quick 2");
    //task_create(db_connection, my_task);
    //bool res = task_get_by_id(db_connection, my_task, 1);
    //db_print_table(db_connection, "task");

    /*
     * START SERVER
     */
    int exit_code = serve(argc, argv);

    //PQfinish(db_connection);
    return exit_code;
}



