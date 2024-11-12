/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        noats is a notes-taking and
                    synchronization application.
License:            MIT
*****************************************************/

#include "../include/noats.h"

/* Noat */

/* convenience */
void noatNew(Noat* noat)
{

}
/* utility */
/* CRUD */
bool noatCreate(PGconn* conn, Noat* noat)
{
    bool success = true;

    const char *stmt = "INSERT INTO public.noats "
        "         (id,      title, author, fmt, body, noat, created, updated)"
        "VALUES   (DEFAULT, $1,    $2,     $3,  $4,   $5,   NULL,    NULL   )"
        "RETURNING id";

    const char **params = malloc(sizeof(void*) * 5);
    params[0] = noat->title;
    params[1] = noat->author;
    params[2] = noat->fmt;
    params[3] = noat->body;
    params[4] = noat->note;

    PGresult *res = PQexecParams(conn, stmt, 5, NULL, params, NULL, NULL, 0); /* string return format */
    if (       PQresultStatus(res) == PGRES_FATAL_ERROR
            || PQresultStatus(res) == PGRES_NONFATAL_ERROR
            || PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error(LOG_TAG, PQresultErrorMessage(res));
        c_log_error(LOG_TAG, PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    else {
        c_log_success(LOG_TAG, "insert (id: %lld)", strtoll(PQgetvalue(res, 0, 0), NULL, 10));
    }

    PQclear(res);
    return success;
}

bool noatUpdate(PGconn* conn, Noat* noat)
{
    bool success = true;
    return success;
}

bool noatDelete(PGconn* conn, i64   id)
{
    bool success = true;
    return success;
}


/* Project */

/* convenience */
void projectNew(Project* noat)
{

}

/* utility */
/* CRUD */
bool projectCreate(PGconn* conn, Project* noat)
{
    bool success = true;
    return success;
}

bool projectUpdate(PGconn* conn, Project* noat)
{
    bool success = true;
    return success;
}

bool projectDelete(PGconn* conn, i64   id)
{
    bool success = true;
    return success;
}

