#include "../include/incl.h"
#include "../include/serv.h"
#include "../include/db.h"
#include "../include/c_log.h"
#include "../include/scrape.h"
#include "../include/task.h"
#include "../include/util.h"
#include "../include/noats.h"
#include "../include/cJSON.h"

/* -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * ----------------------- */

int main(int argc, char *argv[])
{
    int exit_code;
    /* initialization */
    srand(time(NULL));
    c_log_init(stderr, LOG_LEVEL_SUCCESS);
    PGconn* db_connection = db_connect();
    bool sql_script_res = db_exec_script(db_connection, "resources/sql/noats_schema_create.sql");
    if (!sql_script_res)
        return EXIT_FAILURE;

    /* test */
    //task_t my_task;
    //task_new(my_task, "dont be an idiot", "Oskar", "impossible", rand() % 32);
    //task_create(db_connection, my_task);
    //task_get_by_id(db_connection, my_task, 1);

    Noat noat;
    noat.title  = sdscatprintf(sdsempty(), "lol %d", rand() % 256);
    noat.author = sdscatprintf(sdsempty(), "lol %d", rand() % 256);
    noat.fmt    = sdscatprintf(sdsempty(), "lol %d", rand() % 256);
    noat.note   = sdscatprintf(sdsempty(), "lol %d", rand() % 256);
    noat.body   = sdscatprintf(sdsempty(), "lol %d", rand() % 256);
    noatCreate(db_connection, &noat);

    //sds json_sds = sdsfread(sdsempty(), "resources/json/lol.json");
    //printf("%s\n", json_sds);

    //cJSON *json = cJSON_Parse(json_sds);

    //char *json_str = cJSON_Print(json);
    //printf("%s\n", json_str);
    //free(json_str);

    //cJSON_Delete(json);

    db_print_schema_stats(db_connection);


    /* run */
    //exit_code = serve(argc, argv);

    PQfinish(db_connection);
    return exit_code == EXIT_FAILURE ? exit_code : EXIT_SUCCESS;
}



