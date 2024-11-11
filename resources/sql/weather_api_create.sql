/***************************************************************************************************
Procedure:          dbo.usp_DoSomeStuff
Create Date:        2018-01-25
Author:             Joe Expert
Description:        Verbose description of what the query does goes here. Be specific and don't be
                    afraid to say too much. More is better, than less, every single time. Think about
                    "what, when, where, how and why" when authoring a description.
Call by:            [schema.usp_ProcThatCallsThis]
                    [Application Name]
                    [Job]
                    [PLC/Interface]
Affected table(s):  [schema.TableModifiedByProc1]
                    [schema.TableModifiedByProc2]
Used By:            Functional Area this is use in, for example, Payroll, Accounting, Finance
Parameter(s):       @param1 - description and usage
                    @param2 - description and usage
Usage:              EXEC dbo.usp_DoSomeStuff
                        @param1 = 1,
                        @param2 = 3,
                        @param3 = 2
                    Additional notes or caveats about this object, like where is can and cannot be run, or
                    gotchas to watch for when using it.
****************************************************************************************************
SUMMARY OF CHANGES
Date(yyyy-mm-dd)    Author              Comments
------------------- ------------------- ------------------------------------------------------------
2012-04-27          John Usdaworkhur    Move Z <-> X was done in a single step. Warehouse does not
                                        allow this. Converted to two step process.
                                        Z <-> 7 <-> X
                                            1) move class Z to class 7
                                            2) move class 7 to class X

2018-03-22          Maan Widaplan       General formatting and added header information.
2018-03-22          Maan Widaplan       Added logic to automatically Move G <-> H after 12 months.
***************************************************************************************************/

CREATE TABLE IF NOT EXISTS public.location_info (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    /* own fields */
    name               varchar(256),
    region             varchar(256),
    country            varchar(256),
    tz_id              varchar(256),
    localtime_epoch    bigint,
    localtime_loc      varchar(256),
    lon                double precision,
    lat                double precision,
    /* constraints */
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS public.weather_condition (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    /* own fields */
    text_cond          varchar(256),
    icon               varchar(256),
    code               bigint,
    /* constraints */
    PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS public.weather_info (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    weather_info_id       bigint,
    /* own fields */
    last_updated_epoch bigint,
    last_updated       varchar(256),
    temp_c             double precision,
    temp_f             double precision,
    is_day             boolean,
    wind_mph           double precision,
    wind_kph           double precision,
    wind_degree        double precision,
    wind_dir           varchar(256),
    pressure_mb        double precision,
    pressure_in        double precision,
    precip_mm          double precision,
    precip_in          double precision,
    humidity           double precision,
    cloud              double precision,
    feelslike_c        double precision,
    feelslike_f        double precision,
    windchill_c        double precision,
    windchill_f        double precision,
    heatindex_c        double precision,
    heatindex_f        double precision,
    dewpoint_c         double precision,
    dewpoint_f         double precision,
    vis_km             double precision,
    vis_miles          double precision,
    uv                 double precision,
    gust_mph           double precision,
    gust_kph           double precision,
    /* constraints */
    PRIMARY KEY (id),
    FOREIGN KEY (weather_info_id) REFERENCES public.weather_condition (id)
);

CREATE TABLE IF NOT EXISTS public.weather_report (
    /* pkey */
    id                 bigint GENERATED ALWAYS AS IDENTITY,
    /* fkey */
    location_info_id   bigint,
    weather_info_id    bigint,
    /* own fields */
    /* constraints */
    PRIMARY KEY (id),
    FOREIGN KEY (location_info_id) REFERENCES location_info (id),
    FOREIGN KEY (weather_info_id) REFERENCES weather_info (id)
);

