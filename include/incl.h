////////////////////////////////////////////////
// useful includes & typedefs for a c project //
////////////////////////////////////////////////

#ifndef OBHNOTHING_INCLUDES_H
#define OBHNOTHING_INCLUDES_H

/*
 * defaults
 * */
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <wchar.h>
/*
 * linux
 * */
//#if defined(__UNIX__) || defined(__LINUX__)
#if defined(__UNIX__) || defined(__LINUX__)
#include <signal.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
//#endif
#endif

/*
 * external
 * */
//#ifndef STB_DS_IMPLEMENTATION
//#define STB_DS_IMPLEMENTATION
//#include "../external/stb/stb_ds.h"
//#endif

//#include "../external/sds/sds.h"
//#include "../external/sds/sdsalloc.h"

/*
 * math
 * */
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include "../external/cglm/include/cglm/cglm.h"
/*
 * defines
 * */
typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;

typedef  int8_t   i8;
typedef  int16_t  i16;
typedef  int32_t  i32;
typedef  int64_t  i64;

#endif
