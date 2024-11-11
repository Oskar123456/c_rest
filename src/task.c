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

void task_new(task_t t, const char *name, const char *description)
{
    string_t name_s, desc_s;
    string_init(name_s); string_init(desc_s);
    string_set_str(name_s, name); string_set_str(desc_s, description);

    task_init(t);
    task_set_name(t, name_s);
    task_set_description(t, desc_s);
}

bool task_get_by_id(PGconn *conn, task_t task, i64 id)
{
    bool success = true;
    task_clear(task);
    task_init(task);

    const char *stmt_str = "SELECT * FROM public.task "
        "where id = $1 ";

    string_t stmt, id_str;
    string_init(stmt); string_init(id_str);
    string_set_str(stmt, stmt_str); string_printf(id_str, "%lld", id);

    c_log_info("get task id", "%lld", id);

    const char **params = malloc(sizeof(void*) * 1);
    params[0] = string_get_cstr(id_str);

    PGresult *res = PQexecParams(conn, stmt_str, 1, NULL, params, NULL, NULL, 0); /* string return format */
    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
        c_log_error("task_get_by_id", PQerrorMessage(conn));
        c_log_error("task_get_by_id SQLSTATE", PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error("task_get_by_id check return", PQresultErrorMessage(res));
        success = false;
    }

    else {
        for (int i = 0; i < PQnfields(res); ++i) {
            if (strcmp(PQfname(res, i), "name") == 0)
                string_set_str(*task_get_at_name(task), PQgetvalue(res, 0, i));
            if (strcmp(PQfname(res, i), "description") == 0)
                string_set_str(*task_get_at_description(task), PQgetvalue(res, 0, i));
        }
    }

    if (!success)
        task_clear(task);
    else {
        string_t tmp_str;
        string_init(tmp_str);
        task_get_str(tmp_str, task, false);
        c_log_success("task_get_by_id", "retrieved %s", string_get_cstr(tmp_str));
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
        "         (id,      name, description)"
        "VALUES   (DEFAULT, $1,   $2         )"
        "RETURNING id";

    string_t stmt;
    string_init(stmt);
    string_set_str(stmt, stmt_str);

    c_log_info("insert task name",        string_get_cstr(*task_get_at_name(task)));
    c_log_info("insert task description", string_get_cstr(*task_get_at_description(task)));

    const char **params = malloc(sizeof(void*) * 2);
    params[0] = string_get_cstr(*task_get_at_name(task));
    params[1] = string_get_cstr(*task_get_at_description(task));

    PGresult *res = PQexecParams(conn, stmt_str, 2, NULL, params, NULL, NULL, 0); /* string return format */
    if (PQresultStatus(res) == PGRES_FATAL_ERROR || PQresultStatus(res) == PGRES_NONFATAL_ERROR) {
        c_log_error("insert_task", PQerrorMessage(conn));
        c_log_error("insert_task SQLSTATE", PQresultErrorField(res, PG_DIAG_SQLSTATE));
        success = false;
    }

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        c_log_error("insert_task check return", PQresultErrorMessage(res));
        success = false;
    }

    else {
        string_t tmp_str;
        string_init(tmp_str);
        task_get_str(tmp_str, task, false);
        c_log_success("task_create", "insert %s", string_get_cstr(tmp_str));
        string_clear(tmp_str);
    }

    PQclear(res);
    return success;
}

