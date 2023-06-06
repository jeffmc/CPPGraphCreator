#pragma once

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// FORMATTED PRODUCTION CONSOLE OUTPUT
#define PRDERROR(fmt, ...) do { printf ("\x1b[31m" fmt "\x1b[39m\n", ##__VA_ARGS__); } while(0);
#define PRDSUCCESS(fmt, ...) do { printf("\x1b[32m" fmt "\x1b[39m\n", ##__VA_ARGS__); } while(0);
#define PRDWARN(fmt, ...) do { printf("\x1b[33m" fmt "\x1b[39m\n", ##__VA_ARGS__); } while(0);
#define PRDINFO(fmt, ...) do { printf("\x1b[36m" fmt "\x1b[39m\n", ##__VA_ARGS__); } while(0);

// FORMATTED DEBUG-ONLY OUTPUT
#define DBG 0

#if DBG
#define DBGLOG(fmt, ...)   do { printf("\x1b[32m  [LOG] %s:%i %s    " fmt "\x1b[39m\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); } while(0);
#define DBGWARN(fmt, ...)  do { printf("\x1b[33m [WARN] %s:%i %s    " fmt "\x1b[39m\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); } while(0);
#define DBGERROR(fmt, ...) do { printf("\x1b[31m[ERROR] %s:%i %s    " fmt "\x1b[39m\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); } while(0);
#define DBGTRACE()         do { printf("\x1b[36m[TRACE] %s:%i %s\x1b[39m\n",            __FILE__, __LINE__, __PRETTY_FUNCTION__);                } while(0);
#else
#define DBGLOG(fmt, ...)
#define DBGWARN(fmt, ...)
#define DBGERROR(fmt, ...)
#define DBGTRACE()
#endif


// STRING TOOLS
void uppercasify(char* c) {
    DBGTRACE();
    while (*c) {
        *c = toupper(*c);
        ++c;
    }
}

inline bool streq(const char *a, const char *b) { return strcmp(a,b) == 0; }

// MISC
#define UNIMPLEMENTED(what) do { PRDERROR( "UNIMPLEMENTED: " #what ); } while (0)
