#include <iostream>
#include "common.h"

void readline(char* buf, size_t bufsz) {
    std::cin.getline(buf, bufsz);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
	}
	#if 0
	printf("\"%s\"\n", cmdbuf);
	#endif
}

#define CMD_BUF_SZ 256
#define TOKEN_MAX 32

void tokenize(char* str, int& argc, char **argv) {
    char* s = nullptr; // start of token
	char* c = str; // current character, start at beginning of buffer.
	do {
	    if (isspace(*c)) {
	        if (s) { // if start remembered
	          assert(argc < TOKEN_MAX);
	          argv[argc++] = s; // add token to array
	          *c = '\0'; // terminate token
	          s = nullptr; // nullify start ptr
	        }
	    } else {
	        if (s == nullptr) s = c; // remember start if first character of token.
	    }
	} while (*++c != '\0');
	if (s) {
        assert(argc < TOKEN_MAX);
        argv[argc++] = s;
	}
	
	#if DBG
	printf("Tokens (%i):\n", argc);
	for (int i=0;i<argc;++i) {
	    printf("  \"%s\"\n", argv[i]);
	}
	#endif
}