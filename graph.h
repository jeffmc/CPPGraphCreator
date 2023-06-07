#pragma once

#include "common.h"

// GRAPH STRUCTURES

#define VERTEX_LABEL_LENGTH 16

struct Vertex {
    bool free; // to be used in pool
    char label[VERTEX_LABEL_LENGTH+1]; // null-terminated
};

struct Edge { // Edge is directed from a -> b
    bool free; // to be used in pool
    Vertex *a, *b;
    int w;
};

#define VERTEX_POOL_MAX 32
Vertex vertex_pool[VERTEX_POOL_MAX];

#define EDGE_POOL_MAX 992
Edge edge_pool[EDGE_POOL_MAX];

void graph_free_all();
bool valid_label(const char* const label);

int vertex_slots_free();
Vertex* new_vertex(const char* label_src);
void delete_vertex(Vertex* v);
Vertex* find_vertex_by_label(const char* raw_label);

int edge_slots_free();
Edge* new_edge(int weight, Vertex *from, Vertex* to);
void delete_edge(Edge* e);
Edge* find_edge_by_vertices(const Vertex* from, const Vertex* to); // return nullptr if not found
Edge* find_edge_by_labels(const char* from_label, const char* to_label); // return nullptr if not found

int compacted_vertex_array(Vertex* buffer[]); // returns number of vertices
int compacted_edge_array(Edge* buffer[]); // returns number of vertices

#define QUERY_BUFFER_SIZE 64
int find_edges_by_origin(const Vertex* origin, const Edge* edge_buffer[]); // returns number of edges found
int find_edges_by_endpoint(const Vertex* endpoint, const Edge* edge_buffer[]); // returns number of edges found

// IMPLEMENTATION
bool valid_label(const char* const label) {
    DBGTRACE();
    DBGLOG("Validating: %s", label);
    const char* l = label;
    size_t length = strlen(label);
    if (length < 1 || length > VERTEX_LABEL_LENGTH) {
        DBGWARN("Invalid label length: %u", length);
        return false;
    }
    while (*l) {
        char c = *l;
        if ((c < 'A' || c > 'Z') && c != '_') {
            #if DBG
            DBGWARN("Invalid label: \"%s\"!", label);
            #endif
            
            return false;
        }
        
        ++l; 
    }
    DBGLOG("VALIDATED!");
    return true;
}


int vertex_slots_free() {
    int free_slots = 0;
    Vertex* v = vertex_pool;
    while (v < vertex_pool + VERTEX_POOL_MAX) {
        if (v->free) ++free_slots;
        ++v;
    }
    return free_slots;
}


Vertex* new_vertex(const char* label_src) {
    DBGTRACE();
    assert(strlen(label_src) <= VERTEX_LABEL_LENGTH);
    char buf[VERTEX_LABEL_LENGTH+1] = { '\0' };
    strcpy(buf,label_src);
    uppercasify(buf);

    Vertex* v = find_vertex_by_label(buf);
    if (v != nullptr) {
        PRDERROR("Vertex %s already exists!", v->label);
        return nullptr;
    }
    if (!valid_label(buf)) { // TODO: PREVENT DUPLICATES
        PRDERROR("\"%s\" is an invalid vertex label!", buf);
        return nullptr;
    }

    v = vertex_pool;
    while (!v->free) ++v;
    assert(v < vertex_pool + VERTEX_POOL_MAX);
    v->free = false;
    strcpy(v->label, buf);

    return v;
}

void delete_vertex(Vertex* v) {
    DBGTRACE();
    assert(v >= vertex_pool && v < vertex_pool + VERTEX_POOL_MAX);
    v->free = true;
}

Vertex* find_vertex_by_label(const char* raw_label) {
    DBGTRACE();
    char query_label[VERTEX_LABEL_LENGTH+1];
    assert(strlen(raw_label) <= VERTEX_LABEL_LENGTH);
    strcpy(query_label, raw_label);
    uppercasify(query_label);
    if (!valid_label(query_label)) return nullptr;

    Vertex* v = vertex_pool;
    while (v < (vertex_pool + VERTEX_POOL_MAX)) {
        if (!v->free) {
            if (streq(v->label, query_label)) return v;
        }
        ++v;
    }
    return nullptr;
}

int edge_slots_free() {
    int free_slots = 0;
    Edge* e = edge_pool;
    while (e < edge_pool + EDGE_POOL_MAX) {
        if (e->free) ++free_slots;
        ++e;
    }
    return free_slots;
}

Edge* new_edge(int weight, Vertex *from, Vertex* to) {
    DBGTRACE();
    Edge* e = find_edge_by_vertices(from,to);
    if (e) {
        PRDERROR("Edge from %s to %s with %i weight already exists!", from->label, to->label, e->w);
        return nullptr;
    }

    e = edge_pool;
    while (!e->free) ++e;
    assert(e < edge_pool + EDGE_POOL_MAX);
    e->free = false;

    e->a = from;
    e->b = to;
    e->w = weight;
    return e;
}

void delete_edge(Edge* e) {
    DBGTRACE();
    assert(e >= edge_pool && e < edge_pool + EDGE_POOL_MAX);
    assert(e->a >= vertex_pool && e->b < vertex_pool + VERTEX_POOL_MAX);
    e->free = true;
}

Edge* find_edge_by_vertices(const Vertex* from, const Vertex* to) {
    DBGTRACE();
    assert(from >= vertex_pool && from < vertex_pool + VERTEX_POOL_MAX);
    assert(to >= vertex_pool && to < vertex_pool + VERTEX_POOL_MAX);

    Edge* e = edge_pool;
    while (e < edge_pool + EDGE_POOL_MAX) {
        if (!e->free) {
            if (e->a == from && e->b == to) return e;
        }
        ++e;
    }
    return nullptr;
}

Edge* find_edge_by_labels(const char* from_label, const char* to_label) {
    DBGTRACE();
    Vertex *a = find_vertex_by_label(from_label);
    if (a == nullptr) return nullptr;
    Vertex *b = find_vertex_by_label(to_label);
    if (b == nullptr) return nullptr;
    
    return find_edge_by_vertices(a,b);
}

void graph_free_all() {
    DBGTRACE();
    for (int i=0;i<VERTEX_POOL_MAX;++i) vertex_pool[i].free = true;
    for (int i=0;i<EDGE_POOL_MAX;++i) edge_pool[i].free = true;
}

int compacted_vertex_array(Vertex* buffer[]) {
    DBGTRACE();
    int i=0;
    Vertex* p = vertex_pool;
    while (p < vertex_pool + VERTEX_POOL_MAX) {
        if (!p->free) buffer[i++] = p;
        ++p;
    }
    return i;
}

int compacted_edge_array(Edge* buffer[]) {
    int i=0;
    Edge* p = edge_pool;
    while (p < edge_pool + EDGE_POOL_MAX) {
        if (!p->free) buffer[i++] = p;
        ++p;
    }
    return i;
}

// TODO: Dijkstra's algorithm