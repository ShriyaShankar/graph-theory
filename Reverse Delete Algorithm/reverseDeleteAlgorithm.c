#include<stdio.h>
#include<stdlib.h>
#define DEBUG 0
struct pair{
    int u;
    int v;
};
typedef struct pair pair_t;

struct edge{
    int w;
    pair_t* pair;
    struct edge* next;
};
typedef struct edge edge_t;
struct edge_list{
    edge_t* head;
};
typedef struct edge_list edge_list_t;

struct graph{
    int v;
    int** adjmat;
    edge_list_t* list;
};
typedef struct graph graph_t;
void destroy_graph(graph_t* graph){
    for(int i = 0; i < graph->v; i++){
        free(graph->adjmat[i]);
    }
    free(graph->adjmat);
    edge_t* temp = graph->list->head;
    edge_t* temp2 = NULL;
    while(temp != NULL){
        temp2 = temp;
        temp = temp->next;
        free(temp2->pair);
        free(temp2);
        temp2 = NULL;
    }
    free(graph->list);
    graph->list = NULL;
    free(graph);
    graph = NULL;

}
graph_t* initialize_graph(int V){
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->v = V;
    graph->adjmat = (int**)malloc(sizeof(int*)*V);
    for(int i = 0; i < V; i++){
        graph->adjmat[i] = (int*)malloc(sizeof(int)*V);
        for(int j = 0; j < V; j++){
            graph->adjmat[i][j] = 0;
        }
    }
    graph->list = (edge_list_t*)malloc(sizeof(edge_list_t));
    graph->list->head = NULL;
    return graph;
}
edge_t* initialize_edge(int w, pair_t* pair){
    edge_t* edge = (edge_t*)malloc(sizeof(edge_t));
    edge->w = w;
    edge->pair = pair;
    edge->next = NULL;
    return edge;
}
pair_t* initialize_pair(int u, int v){
    pair_t* pair = (pair_t*)malloc(sizeof(pair_t));
    pair->u = u;
    pair->v = v;
    return pair;
}
void addEdge(graph_t* graph, int u, int v, int w){
    graph->adjmat[u][v] = 1;
    graph->adjmat[v][u] = 1;
    pair_t* edge_pair = initialize_pair(u,v);
    edge_t* edge = initialize_edge(w, edge_pair);
    edge_t* temp;
    edge_t* prev = NULL;
    temp = graph->list->head;
    if(temp == NULL){
        graph->list->head = edge;
    }else{
        while(temp!= NULL && temp->w >= edge->w){
            prev = temp;
            temp = temp->next;
        }
        if(prev == NULL){
            edge->next = graph->list->head;
            graph->list->head = edge;
        }else if(temp != NULL && temp->w >= edge->w){
            edge->next = temp->next;
            temp->next = edge;
        }else{
            edge->next = temp;
            prev->next = edge;
        }
        
    }
}

void DFS(int v, int* visited, graph_t* graph){
    visited[v] = 1;
    int i = 0;
    for(i = 0; i < graph->v; i++){
        if(graph->adjmat[v][i] && !visited[i]){
            DFS(i, visited, graph);
        }
    }
}

int isConnected(graph_t* graph){
    int* visited = (int*)malloc(sizeof(int)*graph->v);
    for(int i = 0; i< graph->v; i++){
        visited[i] = 0;
    }
    DFS(0,visited, graph);
    for(int i = 1; i < graph->v; i++){
        if(!visited[i]){
            free(visited);
            return 0;
        }
    }
    free(visited);
    return 1;
}

int reverseDeleteMTS(graph_t* graph){
    int mst_weight = 0;
    edge_t* edge = graph->list->head;
    int u,v;
    while(edge != NULL){
        u = edge->pair->u;
        v = edge->pair->v;
        graph->adjmat[u][v] = 0;
        graph->adjmat[v][u] = 0;
        if(isConnected(graph) == 0){
            graph->adjmat[u][v] = 1;
            graph->adjmat[v][u] = 1;
            printf("(%d,%d)\n",u,v);
            mst_weight += edge->w;
        } 
        edge = edge->next;
    }
    return mst_weight;

}
void display_edges(edge_list_t* list){
    printf("priority edge list\n");
    if(list->head == NULL){
        printf("Empty list\n");
        return;
    }
    edge_t* temp = list->head;
    while(temp != NULL){
        printf("%d %d %d\n",temp->w, temp->pair->u, temp->pair->v);
        temp = temp->next;
    }
    printf("\n");
}
int main(){
    int V,E;
    int u,v,w;
    scanf("%d %d",&V,&E);
    graph_t* graph = initialize_graph(V);
    while(E--){
        scanf("%d %d %d",&u,&v,&w);
        addEdge(graph,u,v,w);
        if(DEBUG){
            display_edges(graph->list);
        }
    }
    
    /*
    addEdge(graph,0,1,4);
    addEdge(graph, 0, 7, 8); 
    addEdge(graph,1, 2, 8); 
    addEdge(graph,1, 7, 11); 
    addEdge(graph,2, 3, 7); 
    addEdge(graph,2, 8, 2); 
    addEdge(graph,2, 5, 4); 
    addEdge(graph,3, 4, 9); 
    addEdge(graph,3, 5, 14); 
    addEdge(graph,4, 5, 10); 
    addEdge(graph,5, 6, 2); 
    addEdge(graph,6, 7, 1); 
    addEdge(graph,6, 8, 6); 
    addEdge(graph,7, 8, 7); 
    */
    printf("Edges in MST\n");
    printf("%d\n",reverseDeleteMTS(graph));
    destroy_graph(graph);
    return 0;
}