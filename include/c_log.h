#ifndef C_LOG_H
#define C_LOG_H

#include "incl.h"

void c_log_init(FILE *out);
void c_log_success(const char* tag, const char* message, ...);
void c_log_debug(const char* tag, const char* message, ...);
void c_log_info(const char* tag, const char* message, ...);
void c_log_warn(const char* tag, const char* message, ...);
void c_log_error(const char* tag, const char* message, ...);

#endif
