#include "../include/incl.h"
#include "../include/c_log.h"
#include "../external/mlib/m-tuple.h"
#include "../external/mlib/m-array.h"
#include "../external/mlib/m-serial-json.h"

#include <endian.h>
#include <postgresql/libpq-fe.h>

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-11-11.............
 * -----------------------
 */

TUPLE_DEF2(task,
         (name,        string_t),
         (author,      string_t),
         (description, string_t),
         (priority,    i16),
         (created,     string_t),
         (updated,     string_t))

TUPLE_DEF2(project,
         (name,        string_t),
         (description, string_t),
         (priority,    i16),
         (created,     string_t),
         (updated,     string_t))

void task_new(task_t t, const char *name, const char *author, const char *description, i16 priority);
bool task_get_by_id(PGconn *conn, task_t task, i64 id);
bool task_create(PGconn *conn, task_t task);

