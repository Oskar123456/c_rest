#include "../include/incl.h"
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
        (name,               string_t),
        (description,        string_t))
#define M_OPL_task_t() TUPLE_OPLIST(task, \
        STRING_OPLIST,  STRING_OPLIST)

void task_new(task_t t, const char *name, const char *description);
bool task_get_by_id(PGconn *conn, task_t task, i64 id);
bool task_create(PGconn *conn, task_t task);
