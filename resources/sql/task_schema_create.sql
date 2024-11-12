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
    description        varchar(256),
    /* constraints */
    PRIMARY KEY (id)
);

