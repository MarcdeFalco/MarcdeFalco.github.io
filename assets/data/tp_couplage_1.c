#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct edge_list {
    int dst;
    struct edge_list *next;
};
typedef struct edge_list edge_list;

struct graph {
    int nvertices;
    bool directed;
    edge_list **edge_lists;
};
typedef struct graph graph;

graph *graph_create(int nvertices, bool directed) {
    graph *g = malloc(sizeof(graph));

    g->nvertices = nvertices;
    g->edge_lists = malloc(sizeof(edge_list *) * nvertices);
    for(int i = 0; i < nvertices; i++)
        g->edge_lists[i] = NULL;
    g->directed = directed;

    return g;
}

void edge_list_free(edge_list *l)
{
    while(l != NULL)
    {
        edge_list *t = l->next;
        free(l);
        l = t;
    }
}

void graph_free(graph *g)
{
    for(int i = 0; i < g->nvertices; i++)
        edge_list_free(g->edge_lists[i]);
    free(g->edge_lists);
    free(g);
}

bool is_connected(graph *g, int x, int y)
{
    edge_list *l = g->edge_lists[x];
    while(l != NULL)
    {
        if(l->dst == y)
            return true;
        l = l->next;
    }
    return false;
}

void add_edge(graph *g, int src, int dst)
{
    edge_list *e = malloc(sizeof(edge_list));
    e->dst= dst;
    e->next = g->edge_lists[src];
    g->edge_lists[src] = e;
    if (!g->directed)
    {
        edge_list *e = malloc(sizeof(edge_list));
        e->dst= src;
        e->next = g->edge_lists[dst];
        g->edge_lists[dst] = e;
    }
}

void add_edges(graph *g, int *vtx_pairs, int npairs)
{
    for(int i = 0; i < npairs*2; i+=2)
    {
        add_edge(g, vtx_pairs[i], vtx_pairs[i+1]);
    }
}

void graph_print(graph *g)
{
    for(int i = 0; i < g->nvertices; i++)
    {
        printf("%d : ", i);
        edge_list *l = g->edge_lists[i];
        while(l != NULL)
        {
            printf(" %d,", l->dst);
            l = l->next;
        }
        printf("\n");
    }
}

int *dfs_init(graph *g, int src)
{
    int *pred = malloc(sizeof(int) * g->nvertices);
    memset(pred, -1, sizeof(int) * g->nvertices);
    pred[src] = src;
    return pred;
}

void dfs(graph *g, int *pred, int x)
{
    edge_list *l = g->edge_lists[x];
    while(l != NULL)
    {
        int y = l->dst;
        if (pred[y] < 0)
        {
            pred[y] = x;
            dfs(g, pred, y);
        }
        l = l->next;
    }
}

int *dfs_launch(graph *g, int src)
{
    int *pred = dfs_init(g, src);
    dfs(g, pred, src);
    return pred;
}

