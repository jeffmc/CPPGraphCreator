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
    "         PATH [src] [dest] - calculate efficient path between two vertices.\n"
    "           EXEC [filename] - run a list of commands from a file.\n"
    "                      QUIT - exit progam.\n";

int main()
{
    free_all(); // sets all graph resources to free state.
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
            printf(helpstr);
        } else if (streq("VERTEX", cmd)) {
            DBGLOG("Command(VERTEX)");
            if (argc != 2) {
                printf("Must specify a valid vertex label!\n");
                continue;
            }
            Vertex* v = new_vertex(argv[1]);
            printf("New vertex: %s\n", v->label);
            UNIMPLEMENTED(VERTEX);
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
            UNIMPLEMENTED(RMALL);
        } else if (streq("PATH", cmd)) {
            DBGLOG("Command(PATH)");
            UNIMPLEMENTED(PATH);
        } else if (streq("EXEC", cmd)) {
            DBGLOG("Command(EXEC)");
            if (argc != 2) {
                printf("Must specify a valid filename!\n");
                continue;
            }
            UNIMPLEMENTED(EXEC);
        } else if (streq("QUIT", cmd) || streq("EXIT", cmd)) {
            DBGLOG("Command(QUIT|EXIT)");
            break;
        }else {
            printf("Command unknown: \"%s\"\n", cmd);
        }
    }
    
    return 0;
}
