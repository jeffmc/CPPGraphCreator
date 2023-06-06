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
    "                    MATRIX - print out an adjacency matrix without weights.\n"
    "                   ADJLIST - print out an adjacency list with weights.\n"
    "                  EDGELIST - print out an edge list with origins, endpoints, and weights.\n"
    "                VERTEXLIST - print out a list of vertices.\n"
    "                      QUIT - exit progam.\n";

void run_cli();
void exec_file(const char* filename);
void print_matrix();

int main(int argc, char* argv[])
{
    graph_free_all(); // sets all graph resources to free state.
    
    if (argc > 1) {
        exec_file(argv[1]);
    } else {
        printf(helpstr);
    }

    run_cli();
    return 0;
}

// Return true if meant to quit.
bool run_command(int argc, char* argv[]) {
    if (argc < 1) return false;
    uppercasify(argv[0]); // command keyword is case-insensitive.
    const char* cmd = argv[0];
    if (streq("HELP", cmd)) {
        printf(helpstr);
    } else if (streq("VERTEX", cmd)) {
        DBGLOG("Command(VERTEX)");
        if (argc != 2) {
            PRDERROR("Must specify a valid vertex label!");
            return false;
        }
        Vertex* v = new_vertex(argv[1]); // Prints out error message if there is one.
        if (v != nullptr) PRDSUCCESS("New vertex: \"%s\"", v->label);
        return false;
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
        graph_free_all();
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
        exec_file(argv[1]);
    } else if (streq("MEM", cmd)) {
        DBGLOG("Command(MEM)");
        PRDINFO("%i/%i vertex slots free\n%i/%i edge slots free", 
            vertex_slots_free(), VERTEX_POOL_MAX, edge_slots_free(), EDGE_POOL_MAX);
    } else if (streq("MATRIX", cmd)) {
        DBGLOG("Command(MATRIX)");
        print_matrix();
    } else if (streq("ADJLIST", cmd)) {
        DBGLOG("Command(ADJLIST)");
        UNIMPLEMENTED(ADJLIST);
    } else if (streq("EDGELIST", cmd)) {
        DBGLOG("Command(EDGELIST)");
        UNIMPLEMENTED(EDGELIST);
    } else if (streq("VERTEXLIST", cmd)) {
        DBGLOG("Command(VERTEXLIST)");
        Vertex* vertbuf[VERTEX_POOL_MAX];
        int vertct = compacted_vertex_array(vertbuf);
        if (vertct == 0) {
            printf("No vertices!\n");
            return false;
        }
        printf("Vertices:\n");
        for (int i=0;i<vertct;++i) {
            printf("  %s\n", vertbuf[i]->label);
        }
        printf("\n");
    } else if (streq("QUIT", cmd) || streq("EXIT", cmd)) {
        DBGLOG("Command(QUIT|EXIT)");
        printf("Quitting...\n");
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

    while (true) {
        printf(">");
        cmdbuf[0] = '\0';
        argc = 0;
        readline(cmdbuf, CMD_BUF_SZ);
        tokenize(cmdbuf, argc, argv);
        if (run_command(argc,argv)) break;
    }
}

void exec_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
        PRDERROR("The file \"%s\" could not be opened/executed!", filename);
    }
    
    char cmdbuf[CMD_BUF_SZ];
    int argc;
    char* argv[TOKEN_MAX];
    while (true) {
        cmdbuf[0] = '\0';
        argc = 0;
        if (fgets(cmdbuf, CMD_BUF_SZ, file) != cmdbuf) {
            if (ferror(file)) {
                PRDERROR("Error encountered reading file: %s", filename);
                fclose(file);
                return;
            }
            if (feof(file)) {
                PRDERROR("Reached end of file unexpectedly: %s", filename);
                fclose(file);
                return;
            }
        }
        tokenize(cmdbuf, argc, argv);
        
        printf("Execute:");
        for (int i=0;i<argc;++i) printf(" %s", argv[i]);
        printf("\n");

        run_command(argc, argv);
        if (feof(file)) break;
    }

    if (file != nullptr) fclose(file);
    PRDSUCCESS("DONE EXECUTING FILE: %s", filename);
}

void print_matrix() {
    DBGTRACE();
    Vertex* vert_array[VERTEX_POOL_MAX];
    int label_lengths[VERTEX_POOL_MAX];
    int vertex_count = compacted_vertex_array(vert_array);
    DBGLOG("Vertex count: %i", vertex_count);
    int max_length = 4;
    for (int i=0;i<vertex_count;++i) {
        int l = strlen(vert_array[i]->label);
        label_lengths[i] = l;
        if (l > max_length) max_length = l;
    }
    for (int y=0;y<max_length;++y) {
        printf("%*c ", max_length, ' ');
        for (int i=0;i<vertex_count;++i) {
            printf("  %c", y<label_lengths[i] ? vert_array[i]->label[y] : ' ');
        }
        printf("\n");
    }
    return;
}