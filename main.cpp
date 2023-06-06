#include <iostream>
#include "command.h"
#include "graph.h"

const char* const helpstr = 
    "                      HELP - this.\n"
    "            VERTEX [label] - add a vertex with label.\n"
    "EDGE [weight] [src] [dest] - add an edge.\n"
    "       RMEDGE [src] [dest] - remove an edge.\n"
    "          RMVERTEX [label] - remove a vertex.\n"
    "                     RMALL - remove entire graph.\n"
    "         PATH [src] [dest] - calculate shortest path between two vertices.\n"
    "           EXEC [filename] - run a list of commands from a file.\n"
    "                       MEM - check how much memory is free for graph storage.\n"
    "                      QUIT - exit progam.\n";

void run_cli();

int main(int argc, char* argv[])
{
    free_all(); // sets all graph resources to free state.
    
    run_cli();
    return 0;
}

// Return true if meant to quit.
bool run_command(int argc, char* argv[]) {
    assert(argc > 0);
    const char* cmd = argv[0];
    if (streq("HELP", cmd)) {
        printf(helpstr);
    } else if (streq("VERTEX", cmd)) {
        DBGLOG("Command(VERTEX)");
        if (argc != 2) {
            PRDERROR("Must specify a valid vertex label!");
            return false;
        }
        Vertex* v = new_vertex(argv[1]);
        if (v != nullptr) {
            PRDSUCCESS("New vertex: \"%s\"", v->label);
        } else {
            PRDERROR("Couldn't create vertex with label \"%s\", likely invalid/duplicate!", argv[1]);
        }
    } else if (streq("EDGE", cmd)) {
        DBGLOG("Command(EDGE)");
        UNIMPLEMENTED(EDGE);
    } else if (streq("RMVERTEX", cmd)) {
        DBGLOG("Command(RMVERTEX)");
        UNIMPLEMENTED(RMVERTEX);
    } else if (streq("RMEDGE", cmd)) {
        DBGLOG("Command(RMEDGE)");
        UNIMPLEMENTED(RMEDGE);
    } else if (streq("RMALL", cmd)) {
        DBGLOG("Command(RMALL)");
        free_all();
        PRDSUCCESS("Removed all vertices/edges.");
    } else if (streq("PATH", cmd)) {
        DBGLOG("Command(PATH)");
        UNIMPLEMENTED(PATH);
    } else if (streq("EXEC", cmd)) {
        DBGLOG("Command(EXEC)");
        if (argc != 2) {
            PRDERROR("Must specify a valid filename!");
            return false;
        }
        UNIMPLEMENTED(EXEC);
    } else if (streq("MEM", cmd)) {
        DBGLOG("Command(MEM)");
        PRDINFO("%i/%i vertex slots free\n%i/%i edge slots free", 
            vertex_slots_free(), VERTEX_POOL_MAX, edge_slots_free(), EDGE_POOL_MAX);
    } else if (streq("QUIT", cmd) || streq("EXIT", cmd)) {
        DBGLOG("Command(QUIT|EXIT)");
        return true;
    } else {
        PRDERROR("Command unknown: \"%s\"", cmd);
    }
    return false;
}

void run_cli() {
    char cmdbuf[CMD_BUF_SZ];
    int argc;
    char* argv[TOKEN_MAX];

    #if DBG
    #else 
    printf(helpstr);
    #endif

    while (true) {
        printf(">");
        cmdbuf[0] = '\0';
        argc = 0;
        parsecmd(cmdbuf, argc, argv);
        if (argc < 1) continue;
        uppercasify(argv[0]); // command keyword is case-insensitive.
        if (run_command(argc,argv)) break;
    }
}