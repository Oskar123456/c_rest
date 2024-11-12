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
void NoatNew(Noat* noat)
{

}
/* utility */
/* CRUD */
bool NoatCreate(PGconn* conn, Noat* noat)
{
    bool success = true;

    const char *stmt_str = "INSERT INTO public.task "
        "         (id,      name, author, description, priority, created, updated)"
        "VALUES   (DEFAULT, $1,   $2,     $3,          $4,       $5,      $6     )"
        "RETURNING id";

//    string_t stmt;
//    string_init_set_str(stmt, stmt_str);
//
//    c_log_info(LOG_TAG, "task name: %s", string_get_cstr(*task_get_at_name(task)));
//
//    string_t priority, created;
//    c_log_timestamp(created);
//    task_set_created(task, created);
//    task_set_updated(task, created);
//    string_init(priority); string_printf(priority, "%d", *task_get_at_priority(task));
//
//    const char **params = malloc(sizeof(void*) * 6);
//    params[0] = string_get_cstr(*task_get_at_name(task));
//    params[1] = string_get_cstr(*task_get_at_author(task));
//    params[2] = string_get_cstr(*task_get_at_description(task));
//    params[3] = string_get_cstr(priority);
//    params[4] = string_get_cstr(*task_get_at_created(task));
//    params[5] = string_get_cstr(*task_get_at_updated(task));
//
//    PGresult *res = PQexecParams(conn, stmt_str, 6, NULL, params, NULL, NULL, 0); /* string return format */
//    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
//        c_log_error(LOG_TAG, PQresultErrorMessage(res));
//        c_log_error(LOG_TAG, PQresultErrorField(res, PG_DIAG_SQLSTATE));
//        success = false;
//    }
//
//    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
//        c_log_error(LOG_TAG, PQresultErrorMessage(res));
//        c_log_error(LOG_TAG, PQresultErrorField(res, PG_DIAG_SQLSTATE));
//        success = false;
//    }
//
//    else {
//        string_t tmp_str;
//        string_init(tmp_str);
//        task_get_str(tmp_str, task, false);
//        c_log_success(LOG_TAG, "insert %s", string_get_cstr(tmp_str));
//        string_clear(tmp_str);
//    }
//
//    string_clear(stmt);
//    string_clear(priority);
//    string_clear(created);
//    PQclear(res);
    return success;
}

bool NoatUpdate(PGconn* conn, Noat* noat)
{

}

bool NoatDelete(PGconn* conn, i64   id)
{

}


/* Project */

/* convenience */
void ProjectNew(Project* noat)
{

}

/* utility */
/* CRUD */
bool ProjectCreate(PGconn* conn, Project* noat)
{

}

bool ProjectUpdate(PGconn* conn, Project* noat)
{

}

bool ProjectDelete(PGconn* conn, i64   id)
{

}

