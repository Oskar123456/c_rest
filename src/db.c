#include "../include/incl.h"
#include "../include/db.h"
#include "../external/mlib/m-string.h"

#include <postgresql/libpq-fe.h>

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-11-08.............
 * -----------------------
 */

static const char *conninfo = "host=localhost "
                              "port=5432 "
                              "dbname=exam_db "
                              "user=postgres "
                              "password=postgres "
                              "connect_timeout=3 ";


static void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

PGconn* db_connect()
{
    PGconn *conn;

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "%s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    /* first, print out the attribute names */
    return conn;
}

void db_print_table(PGconn* conn, const char* table)
{
    PGresult *res;

    string_t stmt;
    string_init(stmt);
    string_printf(stmt, "select * from public.%s", table);

    /* Set always-secure search path, so malicious users can't take control. */
    res = PQexec(conn, "SELECT pg_catalog.set_config('search_path', '', false)");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SET failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);

    res = PQexec(conn, string_get_cstr(stmt));
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    /* better print option */
    PQprintOpt pq_printopts = { 0 };
    pq_printopts.header = true;
    pq_printopts.align = true;
    pq_printopts.fieldSep = "|";
    PQprint(stdout, res, &pq_printopts);

    string_clear(stmt);
    PQclear(res);
}

