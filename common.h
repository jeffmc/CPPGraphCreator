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
#define INTL_DBGPRINT(color, level)           printf("\x1b[" #color "m[" #level "]: %s:%i    %s\x1b[39m\n", __FILE__, __LINE__, __PRETTY_FUNCTION__  );
#define INTL_DBGPRINTNOTE(color, level, note) printf("\x1b[" #color "m[" #level "]: %s:%i    %s: %s\x1b[39m\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, note);
#define DBGLOG(note) INTL_DBGPRINTNOTE(32,LOG,note);
#define DBGWARN(note) INTL_DBGPRINTNOTE(33,WARN,note);
#define DBGERROR(note) INTL_DBGPRINTNOTE(31,ERROR,note);
#define DBGTRACE() INTL_DBGPRINT(36,TRACE);
#else
#define DBGLOG(note)
#define DBGWARN(note)
#define DBGERROR(note)
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
