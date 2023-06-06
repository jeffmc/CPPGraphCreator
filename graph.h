#pragma once

#include "common.h"

// GRAPH STRUCTURES

#define VERTEX_LABEL_LENGTH 16

bool valid_label(const char* const label) {
    DBGTRACE();
    const char* l = label;
    while (*l) {
        char c = *l;
        if ((c < 'A' || c > 'Z') && c != '_') {
            #if DBG
            char dbgbuf[256] = { '\0' };
            snprintf(dbgbuf,256,"Invalid label: \"%s\"!", label);
            DBGWARN(dbgbuf);
            #endif

            return false;
        }
        ++l; 
    }
    return true;
}

struct Vertex {
    bool free; // to be used in pool
    char label[VERTEX_LABEL_LENGTH+1]; // null-terminated
};

#define VERTEX_POOL_MAX 128
Vertex vertex_pool[VERTEX_POOL_MAX];

int vertex_slots_free() {
    int free_slots = 0;
    Vertex* v = vertex_pool;
    while (v < vertex_pool + VERTEX_POOL_MAX) {
        if (v->free) ++free_slots;
        ++v;
    }
    return free_slots;
}

#if DBG
#define LOGVERTEXSLOTS() \
do { \
    char buf[64] = { '\0' }; \
    snprintf(buf,32,"%i/%i VERTEX SLOTS FREE", vertex_slots_free(), VERTEX_POOL_MAX); \
    DBGLOG(buf); \
} while (0);
#else
#define LOGVERTEXSLOTS()
#endif

Vertex* new_vertex(const char* label_src) {
    DBGTRACE();
    Vertex* v = vertex_pool;
    while (!v->free) ++v;
    assert(v < vertex_pool + VERTEX_POOL_MAX);
    v->free = false;

    assert(strlen(label_src) <= VERTEX_LABEL_LENGTH);
    strcpy(v->label,label_src);
    uppercasify(v->label);
    if (!valid_label(v->label)) { // TODO: PREVENT DUPLICATES
        v->free = true;
        return nullptr;
    }

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
    while (v < vertex_pool + VERTEX_POOL_MAX) {
        if (!v->free) {
            if (streq(v->label, query_label)) return v;
        }
    }
    return nullptr;
}

struct Edge { // Edge is directed from a -> b
    bool free; // to be used in pool
    Vertex *a, *b;
    int w;
};

#define EDGE_POOL_MAX 1024
Edge edge_pool[EDGE_POOL_MAX];

int edge_slots_free() {
    int free_slots = 0;
    Edge* e = edge_pool;
    while (e < edge_pool + EDGE_POOL_MAX) {
        if (e->free) ++free_slots;
        ++e;
    }
    return free_slots;
}

#if DBG
#define LOGEDGESLOTS() \
do { \
    char buf[64] = { '\0' }; \
    snprintf(buf,32,"%i/%i EDGE SLOTS FREE", edge_slots_free(), EDGE_POOL_MAX); \
    DBGLOG(buf); \
} while (0);
#else
#define LOGEDGESLOTS()
#endif

Edge* new_edge(int weight, Vertex *from, Vertex* to) {
    DBGTRACE();
    Edge* e = edge_pool;
    while (!e->free) ++e;
    assert(e < edge_pool + EDGE_POOL_MAX);
    e->free = false;

    // TODO: PREVENT DUPLICATES
    
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
    }
    return nullptr;
}

Edge* find_edge_by_labels(const char* from_label, const char* to_label) {
    DBGTRACE();
    Vertex *a = find_vertex_by_label(from_label);
    if (!a) return nullptr;
    Vertex *b = find_vertex_by_label(to_label);
    if (!b) return nullptr;
    
    return find_edge_by_vertices(a,b);
}

void free_all() {
    DBGTRACE();
    for (int i=0;i<VERTEX_POOL_MAX;++i) vertex_pool[i].free = true;
    for (int i=0;i<EDGE_POOL_MAX;++i) edge_pool[i].free = true;
}

// TODO: Dijkstra's algorithm