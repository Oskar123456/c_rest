#include "../include/task.h"

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-11-11.............
 * -----------------------
 */

void task_new(task_t t, const char *name, const char *author, const char *description, i16 priority)
{
    string_t name_s, author_s, desc_s;
    string_init_set_str(name_s,   name);
    string_init_set_str(author_s, author);
    string_init_set_str(desc_s,   description);

    task_init(t);
    task_set_name(t, name_s);
    task_set_author(t, author_s);
    task_set_description(t, desc_s);
    task_set_priority(t, priority);

    c_log_info(LOG_TAG, "new task (%s)", name);
}

bool task_get_by_id(PGconn *conn, task_t task, i64 id)
{
    bool success = true;
    task_init(task);

    const char *stmt_str = "SELECT * FROM public.task where id = $1 ";

    string_t stmt, id_str;
    string_init(stmt); string_init(id_str);
    string_set_str(stmt, stmt_str); string_printf(id_str, "%lld", id);

    const char **params = malloc(sizeof(void*) * 1);
    params[0] = string_get_cstr(id_str);

    PGresult *res = PQexecParams(conn, stmt_str, 1, NULL, params, NULL, NULL, 0); /* string return format */
    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
        c_log_error(LOG_TAG, "%s (%s)", PQresultErrorMessage(res), PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error(LOG_TAG, "%s (%s)", PQresultErrorMessage(res), PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    else {
        for (int i = 0; i < PQnfields(res); ++i) {
            if (strcmp(PQfname(res, i), "name") == 0)
                string_set_str(*task_get_at_name(task), PQgetvalue(res, 0, i));
            if (strcmp(PQfname(res, i), "author") == 0)
                string_set_str(*task_get_at_author(task), PQgetvalue(res, 0, i));
            if (strcmp(PQfname(res, i), "description") == 0)
                string_set_str(*task_get_at_description(task), PQgetvalue(res, 0, i));
            if (strcmp(PQfname(res, i), "priority") == 0)
                task_set_priority(task, strtol(PQgetvalue(res, 0, i), NULL, 10));
            if (strcmp(PQfname(res, i), "created") == 0)
                string_set_str(*task_get_at_created(task), PQgetvalue(res, 0, i));
            if (strcmp(PQfname(res, i), "updated") == 0)
                string_set_str(*task_get_at_updated(task), PQgetvalue(res, 0, i));
        }
    }

    if (!success)
        task_clear(task);
    else {
        string_t tmp_str;
        string_init(tmp_str);
        task_get_str(tmp_str, task, false);
        c_log_success(LOG_TAG, "retrieved %s", string_get_cstr(tmp_str));
        string_clear(tmp_str);
    }

    string_clear(stmt);
    string_clear(id_str);
    PQclear(res);
    return success;
}

bool task_create(PGconn *conn, task_t task)
{
    bool success = true;

    const char *stmt_str = "INSERT INTO public.task "
        "         (id,      name, author, description, priority, created, updated)"
        "VALUES   (DEFAULT, $1,   $2,     $3,          $4,       $5,      $6     )"
        "RETURNING id";

    string_t stmt;
    string_init_set_str(stmt, stmt_str);

    c_log_info(LOG_TAG, "task name: %s", string_get_cstr(*task_get_at_name(task)));

    string_t priority, created;
    c_log_timestamp(created);
    task_set_created(task, created);
    task_set_updated(task, created);
    string_init(priority); string_printf(priority, "%d", *task_get_at_priority(task));

    const char **params = malloc(sizeof(void*) * 6);
    params[0] = string_get_cstr(*task_get_at_name(task));
    params[1] = string_get_cstr(*task_get_at_author(task));
    params[2] = string_get_cstr(*task_get_at_description(task));
    params[3] = string_get_cstr(priority);
    params[4] = string_get_cstr(*task_get_at_created(task));
    params[5] = string_get_cstr(*task_get_at_updated(task));

    PGresult *res = PQexecParams(conn, stmt_str, 6, NULL, params, NULL, NULL, 0); /* string return format */
    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
        c_log_error(LOG_TAG, PQresultErrorMessage(res));
        c_log_error(LOG_TAG, PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error(LOG_TAG, PQresultErrorMessage(res));
        c_log_error(LOG_TAG, PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    else {
        string_t tmp_str;
        string_init(tmp_str);
        task_get_str(tmp_str, task, false);
        c_log_success(LOG_TAG, "insert %s", string_get_cstr(tmp_str));
        string_clear(tmp_str);
    }

    string_clear(stmt);
    string_clear(priority);
    string_clear(created);
    PQclear(res);
    return success;
}

