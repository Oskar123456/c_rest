/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        noats is a notes-taking and
                    synchronization application.
License:            MIT
*****************************************************/

#include "incl.h"
#include "c_log.h"
#include <postgresql/libpq-fe.h>

typedef struct Noat Noat;
typedef struct Project Project;

struct Noat {
    i64      id;
    sds      title;
    sds      author;
    sds      fmt;
    sds      body;
    sds      note;
    sds      created;
    sds      updated;
    Project* projects;
};

struct Project {
    i64   id;
    sds   title;
    sds   author;
    sds   description;
    sds   created;
    sds   updated;
    Noat* noats;
};

/* Noat */

/* convenience */
void NoatNew(Noat* noat);
/* utility */
/* CRUD */
bool NoatCreate(PGconn* conn, Noat* noat);
bool NoatUpdate(PGconn* conn, Noat* noat);
bool NoatDelete(PGconn* conn, i64   id);

/* Project */

/* convenience */
void ProjectNew(Project* noat);
/* utility */
/* CRUD */
bool ProjectCreate(PGconn* conn, Project* noat);
bool ProjectUpdate(PGconn* conn, Project* noat);
bool ProjectDelete(PGconn* conn, i64   id);
