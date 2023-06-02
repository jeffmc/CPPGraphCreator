#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>

void uppercasify(char* c) {
    while (*c) {
        *c = toupper(*c);
        ++c;
    }
}
inline bool streq(const char *a, const char *b) { return strcmp(a,b) == 0; }
#define UNIMPLEMENTED(what) printf("UNIMPLEMENTED \"" #what "\": L%i\n", __LINE__);


// COMMAND PARSING

#define CMD_BUF_SZ 256
#define TOKEN_MAX 32

void parsecmd(char* cmdbuf, int& argc, char **argv) {    
	std::cin.getline(cmdbuf, CMD_BUF_SZ);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
	}
	#if 0
	printf("\"%s\"\n", cmdbuf);
	#endif
	
	// TOKENIZATION
	
	char* s = nullptr; // start of token
	char* c = cmdbuf; // current character, start at beginning of buffer.
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
	
	#if 0
	printf("Tokens (%i):\n", argc);
	for (int i=0;i<argc;++i) {
	    printf("  \"%s\"\n", argv[i]);
	}
	#endif
}

// END OF COMMAND PARSING

// GRAPH STRUCTURES

#define VERTEX_LABEL_LENGTH 16

struct Vertex {
    char label[VERTEX_LABEL_LENGTH+1]; // null-terminated
};

Vertex* new_vertex(const char* label_src) {
    Vertex* v = new Vertex;
    assert(strlen(label_src) < VERTEX_LABEL_LENGTH);
    strcpy(v->label,label_src);
    uppercasify(v->label);
    return v;
}

struct Edge { // Edge is directed from a -> b
    Vertex *a, *b;
    int w;
};

Edge* new_edge(int weight, Vertex *from, Vertex* to) {
    Edge* e = new Edge;
    e->a = from;
    e->b = to;
    e->w = weight;
    return e;
} 

// END OF GRAPH STRUCTURES

int main()
{
    char cmdbuf[CMD_BUF_SZ];
    int argc;
    char* argv[TOKEN_MAX];
    while (true) {
        printf("> ");
        cmdbuf[0] = '\0';
        argc = 0;
        parsecmd(cmdbuf, argc, argv);
        if (argc < 1) continue;
        char* cmd = argv[0];
        uppercasify(cmd);
        if (streq("HELP", cmd)) {
            printf(
                "                      HELP - this.\n"
                "            VERTEX [label] - add a vertex with label.\n"
                "EDGE [weight] [src] [dest] - add an edge.\n"
                "       RMEDGE [src] [dest] - remove an edge.\n"
                "          RMVERTEX [label] - remove a vertex.\n"
                "         PATH [src] [dest] - calculate efficient path between two vertices.\n"
                "                      QUIT - exit progam.\n");
        } else if (streq("VERTEX", cmd)) {
            if (argc != 2) {
                printf("Must specify a valid vertex label!\n");
                continue;
            }
            Vertex* v = new_vertex(argv[1]);
            printf("New vertex: %s\n", v->label);
            UNIMPLEMENTED(VERTEX);
        } else if (streq("EDGE", cmd)) {
            UNIMPLEMENTED(EDGE);
        } else if (streq("RMVERTEX", cmd)) {
            UNIMPLEMENTED(RMVERTEX);
        } else if (streq("RMEDGE", cmd)) {
            UNIMPLEMENTED(RMEDGE);
        } else if (streq("PATH", cmd)) {
            UNIMPLEMENTED(PATH);
        } else if (streq("QUIT", cmd) || streq("EXIT", cmd)) {
            break;
        }else {
            printf("Command unknown: \"%s\"\n", cmd);
        }
    }
    
    return 0;
}
