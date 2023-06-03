#pragma once

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define DBG 1

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

void uppercasify(char* c) {
    DBGTRACE();
    while (*c) {
        *c = toupper(*c);
        ++c;
    }
}

inline bool streq(const char *a, const char *b) { return strcmp(a,b) == 0; }

#define UNIMPLEMENTED(what) do { DBGERROR( "UNIMPLEMENTED: " #what ); } while (0)
