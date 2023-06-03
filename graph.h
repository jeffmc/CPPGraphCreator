#pragma once

#include "common.h"

// GRAPH STRUCTURES

#define VERTEX_LABEL_LENGTH 16

bool valid_label(const char* const label) {
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

Vertex* new_vertex(const char* label_src) {
    DBGTRACE();
    Vertex* v = vertex_pool;
    while (!v->free) ++v;
    assert(v < vertex_pool + VERTEX_POOL_MAX);
    v->free = false;

    assert(strlen(label_src) <= VERTEX_LABEL_LENGTH);
    strcpy(v->label,label_src);
    uppercasify(v->label);
    if (!valid_label(v->label)) {
        v->free = true;
        return nullptr;
    }

    return v;
}

void delete_vertex(Vertex* v) {
    assert(v >= vertex_pool && v < vertex_pool + VERTEX_POOL_MAX);
    v->free = true;
}

Vertex* find_vertex_by_label(const char* raw_label) {
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

Edge* new_edge(int weight, Vertex *from, Vertex* to) {
    Edge* e = edge_pool;
    while (!e->free) ++e;
    assert(e < edge_pool + EDGE_POOL_MAX);
    e->free = false;
    
    e->a = from;
    e->b = to;
    e->w = weight;
    return e;
}

void delete_edge(Edge* e) {
    assert(e >= edge_pool && e < edge_pool + EDGE_POOL_MAX);
    e->free = true;
}

Edge* find_edge_by_vertices(const Vertex* from, const Vertex* to) {
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
    Vertex *a = find_vertex_by_label(from_label);
    if (!a) return nullptr;
    Vertex *b = find_vertex_by_label(to_label);
    if (!b) return nullptr;
    
    return find_edge_by_vertices(a,b);
}

void free_all() {
    for (int i=0;i<VERTEX_POOL_MAX;++i) vertex_pool[i].free = true;
    for (int i=0;i<EDGE_POOL_MAX;++i) edge_pool[i].free = true;
}