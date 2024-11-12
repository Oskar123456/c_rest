/***************************************************************************************************
Procedure:          taskdb_ddl
Create Date:        2024-11-12
Author:             Oskar
Description:        data definition for task project.
Call by:            [main.c]
Affected table(s):  [public.task]
Used By:            core
Parameter(s):       []
Usage:              EXEC $(proc_name)
****************************************************************************************************
SUMMARY OF CHANGES
Date(yyyy-mm-dd)    Author              Comments
------------------- ------------------- ------------------------------------------------------------
2024-11-12          Oskar               prettied.
***************************************************************************************************/

CREATE TABLE IF NOT EXISTS public.task (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    /* own fields */
    name               varchar(256),
    author             varchar(256),
    description        text,
    priority           smallint,
    created            timestamp default current_timestamp, /* [without timezone] */
    updated            timestamp, /* [without timezone] */
    /* constraints */
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS public.project (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    /* own fields */
    name               varchar(256),
    description        text,
    priority           smallint,
    created            timestamp default current_timestamp, /* [without timezone] */
    updated            timestamp, /* [without timezone] */
    /* constraints */
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS public.task_project_assoc (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    task_id            bigint,
    project_id         bigint,
    /* own fields */
    /* constraints */
    PRIMARY KEY (id),
    FOREIGN KEY (task_id)    REFERENCES public.task (id),
    FOREIGN KEY (project_id) REFERENCES public.project (id)
);

CREATE OR REPLACE FUNCTION update_modified_column() /* auto-timestamp */
RETURNS TRIGGER AS $$
BEGIN
   IF row(NEW.*) IS DISTINCT FROM row(OLD.*) THEN
      NEW.modified = now();
      RETURN NEW;
   ELSE
      RETURN OLD;
   END IF;
END;

CREATE TRIGGER
    update_task_timestamp
BEFORE UPDATE ON
    task
FOR EACH ROW EXECUTE PROCEDURE
    update_modified_column();
