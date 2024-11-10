#ifndef C_LOG_H
#define C_LOG_H

#include "incl.h"

typedef enum LOG_LEVEL {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_SUCCESS,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
} LOG_LEVEL;

/**
 * initialize c_log, must be called before use
 * @param out output file pointer
 * @param level anything below this threshold will be ignored
 * */
void c_log_init(FILE *out, LOG_LEVEL level);
/**
 * log success
 * */
void c_log_success(const char* tag, const char* message, ...);
void c_log_debug(const char* tag, const char* message, ...);
void c_log_info(const char* tag, const char* message, ...);
void c_log_warn(const char* tag, const char* message, ...);
void c_log_error(const char* tag, const char* message, ...);

#endif