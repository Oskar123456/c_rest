#include "../include/c_log.h"

/*
 * -----------------------
 * RESTfulness in C.......
 * ***********************
 * Oskar Bahner Hansen....
 * cph-oh82@cphbusiness.dk
 * 2024-10-31.............
 * -----------------------
 */

const char* ansi_color_arr[] = {
    "\x1b[0m",
    "\x1b[32m",
    "\x1b[34m",
    "\x1b[33m",
    "\x1b[31m",
};

const char* log_level_translations[] = {
    "DEBUG",
    "SUCCESS",
    "INFO",
    "WARNING",
    "ERROR",
};

typedef enum ANSI_COLOR {
    ANSI_RESET,
    ANSI_RED,
    ANSI_YELLOW,
    ANSI_GREEN,
    ANSI_BLUE,
} ANSI_COLOR;

typedef enum LOG_LEVEL {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_SUCCESS,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
} LOG_LEVEL;

static FILE *output_file;

void c_log_init(FILE *out) { output_file = out; }

void c_log(const char* tag, LOG_LEVEL level, const char* message, va_list args)
{
    char buffer[32] = { 0 };
    int millisec;
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%H:%M:%S", tm_info);
    fprintf(output_file, "%s", ansi_color_arr[level]);
    fprintf(output_file, "[%s] [%s.%03d] ", log_level_translations[level], buffer, millisec);
    fprintf(output_file, "[%s]: ", tag);
    fprintf(output_file, "%s", ansi_color_arr[0]);
    vfprintf(output_file, message, args);
    fprintf(output_file, "\n");
    fflush(output_file);
}

void c_log_debug(const char* tag, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    c_log(tag, LOG_LEVEL_DEBUG, message, args);
    va_end(args);
}

void c_log_info(const char* tag, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    c_log(tag, LOG_LEVEL_INFO, message, args);
    va_end(args);
}

void c_log_warn(const char* tag, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    c_log(tag, LOG_LEVEL_WARNING, message, args);
    va_end(args);
}

void c_log_error(const char* tag, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    c_log(tag, LOG_LEVEL_ERROR, message, args);
    va_end(args);
}

void c_log_success(const char* tag, const char* message, ...)
{
    va_list args;
    va_start(args, message);
    c_log(tag, LOG_LEVEL_SUCCESS, message, args);
    va_end(args);
}
