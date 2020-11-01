#include<stdio.h>
#include<stdlib.h>
#define DEBUG 0
struct vertex{
    int color;
    int id;
    int neighbours;
    struct vertex* next;
};
typedef struct vertex vertex_t;
struct vertex_list{
    vertex_t* head;
};
typedef struct vertex_list vertex_list_t;
struct graph{
    int v;
    int** adjmat;
    vertex_list_t* list;
    int colored;
};
typedef struct graph graph_t;
vertex_list_t* initialize_list(){
    vertex_list_t* list = (vertex_list_t*)malloc(sizeof(vertex_list_t));
    list->head = NULL;
    return list;
}
void destroy_graph(graph_t* graph){
    for(int i = 0; i < graph->v; i++){
        free(graph->adjmat[i]);
    }
    free(graph->adjmat);
    vertex_t* temp = graph->list->head;
    vertex_t* temp2 = NULL;
    while(temp != NULL){
        temp2 = temp;
        temp = temp->next;
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
    for(int i =0; i < V; i++){
        graph->adjmat[i] = (int*)malloc(sizeof(int)*V);
        for(int j = 0; j < V; j++){
            graph->adjmat[i][j] = 0;
        }
    }
    graph->list = initialize_list();
    graph->colored = 0;
    return graph;
}
vertex_t* initialize_vertex(int id, int neighbours){
    vertex_t* new_v = (vertex_t*)malloc(sizeof(vertex_t));
    new_v->id = id;
    new_v->color = -1;
    new_v->neighbours = neighbours;
    new_v->next = NULL;
}
void addEdge(graph_t* graph, int u, int v){
    graph->adjmat[u][v] = 1;
    graph->adjmat[v][u] = 1;

}
void addVertex(vertex_list_t* list, vertex_t* new_v){
    vertex_t* temp = list->head;
    vertex_t* prev = NULL;
    if(temp == NULL){
        list->head = new_v;
    }else{
        while(temp!= NULL && temp->neighbours >= new_v->neighbours){
            prev = temp;
            temp = temp->next;
        }
        if(prev == NULL){
            new_v->next = list->head;
            list->head = new_v;
        }else if(temp != NULL && temp->neighbours >= new_v->neighbours){
            new_v->next = temp->next;
            temp->next = new_v;
        }else{
            new_v->next = temp;
            prev->next = new_v;
        }
        
    }
}
void display_list(vertex_list_t* list){
    printf("priority vertex list\n");
    if(list->head == NULL){
        printf("Empty list\n");
        return;
    }
    vertex_t* temp = list->head;
    while(temp != NULL){
        printf("%d %d %d\n",temp->id, temp->neighbours, temp->color);
        temp = temp->next;
    }
    printf("\n");
}
void get_neighbours(graph_t* graph){
    int i,j,neighbours;
    for(i = 0; i < graph->v; i++){
        neighbours = 0;
        for(j = 0; j < graph->v; j++){
            if(graph->adjmat[i][j]){
                ++neighbours;
            }
        }
        vertex_t* new_v = initialize_vertex(i,neighbours);
        addVertex(graph->list,new_v);
    }
    if(DEBUG){
        display_list(graph->list);
    }
}
vertex_t** build_array(graph_t* graph){
    vertex_t** array = (vertex_t**)malloc(sizeof(vertex_t*)*graph->v);
    vertex_t* temp = graph->list->head;
    for(int i = 0; i < graph->v; i++){
        array[i] = temp;
        temp = temp->next;
    }
    return array;
}

void color_graph(graph_t* graph, vertex_t** array){
   int i,j,u,v,k;
   int color = 0;
   int same = 0;
   for(i = 0; i < graph->v; i++){
        //check if vertex is colored
        if(array[i]->color != -1)
            continue;
        //not colored
        if(DEBUG){
            printf("NOT colored %d, current color %d \n",i,color+1);
        }
        //give color
        array[i]->color = color;
        for(j = i+1; j < graph->v; j++){
            if(!graph->adjmat[array[i]->id][array[j]->id] && array[j]->color == -1){
                array[j]->color = color;
                //check neighbours
                same = 0;
                for(k = 0; k < graph->v; k++){
                    if(graph->adjmat[array[j]->id][array[k]->id] && array[j]->color == array[k]->color){
                        if(DEBUG){
                            printf("%d and %d have same color\n",array[j]->id,array[k]->id);
                        }
                        same = 1;
                        break;
                    }
                }
                if(same){
                    array[j]->color = -1;
                }
            }   
        }
        ++color;
   }
}

/*
void color_graph(graph_t* graph, vertex_t** array){
    int i,j,k;
    int color = 0;
    while(graph->colored < graph->v){
        for(i = 0; i < graph->v; i++){
            if(array[i]->color == -1){
                break;
            }
        }
        array[i]->color = color;
        for(j = 0; j < graph->v; j++){
            if(!graph->adjmat[array[i]->id][array[j]->id] && array[j]->color == -1){

            }
        }
    }
}
*/
int main(){
    int V,E;
    int u,v;
    scanf("%d %d",&V,&E);
    graph_t* graph = initialize_graph(V);
    while(E--){
        scanf("%d %d",&u,&v);
        addEdge(graph,u,v);
    }
    get_neighbours(graph);
    vertex_t** vertex_array = build_array(graph);
    color_graph(graph, vertex_array);
    for(int i = 0; i < graph->v; i++){
        printf("vertex %d color is %d\n", vertex_array[i]->id, vertex_array[i]->color+1);
    }
    destroy_graph(graph);
    free(vertex_array);
    return 0;
}