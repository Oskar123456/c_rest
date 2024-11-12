/*****************************************************
Create Date:        2024-11-12
Author:             Oskar Bahner Hansen
Email:              cph-oh82@cphbusiness.dk
Description:        noats is a notes-taking and
                    synchronization application.
License:            MIT
*****************************************************/

#ifndef NOATS_H
#define NOATS_H

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
void noatNew(Noat* noat);
/* utility */
/* CRUD */
bool noatCreate(PGconn* conn, Noat* noat);
bool noatUpdate(PGconn* conn, Noat* noat);
bool noatDelete(PGconn* conn, i64   id);

/* Project */

/* convenience */
void projectNew(Project* noat);
/* utility */
/* CRUD */
bool projectCreate(PGconn* conn, Project* noat);
bool projectUpdate(PGconn* conn, Project* noat);
bool projectDelete(PGconn* conn, i64   id);

#endif
