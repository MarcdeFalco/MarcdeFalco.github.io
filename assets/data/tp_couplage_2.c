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

graph *residual_graph(graph *g, bool *sources, int *matching)
{
    graph *gr = graph_create(g->nvertices, true);
    for(int i = 0; i < g->nvertices; i++)
    {
        edge_list *l = g->edge_lists[i];
        while(l != NULL)
        {
            if((sources[i] && matching[i] != l->dst)
               || (!sources[i] && matching[i] == l->dst))
                add_edge(gr, i, l->dst);
            l = l->next;
        }
    }
    return gr;
}

void swap_along_path(int *matching, int *pred, int x)
{
    bool swap = true;
    while(pred[x] != x)
    {
        if(swap)
        {
            matching[x] = pred[x];
            matching[pred[x]] = x;
        }
        swap = !swap;
        x = pred[x];
    }
}

bool augment(graph *g, bool *sources, int *matching, int *pred)
{
    for(int i =0; i < g->nvertices; i++)
    {
        if(!sources[i] && matching[i] < 0 && pred[i] >= 0)
        {
            swap_along_path(matching, pred, i);
            return true;
        }
    }

    return false;
}

int *maximal_matching(graph *g, bool *sources)
{
    int *matching = malloc(sizeof(int) * g->nvertices); 
    memset(matching, -1, sizeof(int) * g->nvertices);
    for(int i = 0; i < g->nvertices; i++)
    {
        if(sources[i] && matching[i] < 0)
        {
            graph *gr = residual_graph(g, sources, matching);
            int *pred =dfs_launch(gr, i);
            augment(g, sources, matching, pred);
            free(pred);
            graph_free(gr);
        }
    }
    return matching;
}

int main(void)
{
    /*
    graph *g = graph_create(5, false);
    int vtx_pairs[] = { 0, 3, 1, 3, 1, 4, 2, 3, 2, 4 };
    bool sources[] = { true, true, true, false, false };
    */
    graph *g = graph_create(12, false);
    int vtx_pairs[] = { 0, 8, 0, 7, 2, 6, 2, 9, 3, 8, 4, 8, 4, 9, 5, 11 };
    bool sources[] = { true, false, true, true, true, true, 
        false, false, false, false, false, false };

    add_edges(g, vtx_pairs, 8);
    graph_print(g);

    int *matching = maximal_matching(g, sources);
    for(int i = 0; i < g->nvertices; i++)
    {
        printf("%d ", matching[i]); 
    }
    printf("\n");

    graph_free(g);
}
