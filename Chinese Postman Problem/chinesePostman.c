#include<stdio.h>
#include<stdlib.h>

typedef struct node {
    int id;
    int cost;
    struct node* next;
} node_t;

typedef struct list {
    node_t* head;
    node_t* tail;
    int num_nodes;
} list_t;

node_t* create_node(int id, int cost) {
    node_t* newNode = malloc(sizeof(node_t));
    newNode -> id = id;
    newNode -> cost = cost;
    newNode -> next = NULL;
    return newNode;
}

void push_back(list_t* list, int id, int cost) {
    node_t* newNode = create_node(id, cost);
    if(list -> head == NULL) {
        list -> head = newNode;
        list -> num_nodes = 1;
    }        
    else {
        list -> tail -> next = newNode;
        ++list -> num_nodes;
    }        
    list -> tail = newNode;
}


int min(int a, int b) {
    return a < b ? a: b;
}

void dijkstra(int source, int n, int** minCost, list_t* graph) {
    
    int* costs = minCost[source];
    
    for(int i = 0; i <= n; ++i) costs[i] = 1e9;
    
    costs[source] = 0;
    
    int* vis = calloc(n + 1, sizeof(int));

    int cur = source;
    int costs_found = 0;

    while(costs_found < n) {
                
        ++costs_found;
        vis[cur] = 1;

        node_t* cur_node = graph[cur].head;
        while(cur_node) {
            costs[cur_node -> id] = min (
                costs[cur_node -> id], 
                costs[cur] + cur_node -> cost
            );
            cur_node = cur_node -> next;
        }

        int next_node = cur;
        for(int i=1; i<=n; ++i)
            if(vis[i] == 0 && ( costs[i] < costs[next_node] || next_node == cur ))
                next_node = i;

        cur = next_node;
    }
    
    free(vis);
}


// backtracking method to find all possible pairs among the nodes with odd degree and find the min cost among them
void find_min_pair_cost(int* odd_nodes, int num_odd_nodes, int** costs, int* vis, int cur_cost, int* min_cost_of_pairs, int cur) {    
    if(cur == num_odd_nodes)
        *min_cost_of_pairs = min(*min_cost_of_pairs, cur_cost);        
    else {
        if(vis[cur] == 1) find_min_pair_cost(odd_nodes, num_odd_nodes, costs, vis, cur_cost, min_cost_of_pairs, cur + 1);
        else {
            vis[cur] = 1;
            for(int i = cur + 1; i < num_odd_nodes; ++i) {
                if(vis[i] == 0) {                    
                    vis[i] = 1;
                    find_min_pair_cost(odd_nodes, num_odd_nodes, costs, vis, cur_cost + costs[odd_nodes[cur]][odd_nodes[i]], min_cost_of_pairs, cur + 1);
                    vis[i] = 0;                    
                }
            }
            vis[cur] = 1;
        }
    }
}

int main() {
    int n, e;
    scanf("%d %d", &n, &e);

    // allocate adjacency list
    list_t* graph = malloc((n + 1) * sizeof(list_t));

    // allocate minCost array
    int** minCost = malloc((n + 1) * sizeof(int*));
    for(int i = 0; i <= n; ++i) minCost[i] = malloc((n + 1) * sizeof(int));

    int total_cost = 0;

    for(int i = 0; i < e; ++i) {
        int u, v, cost;
        scanf("%d %d %d", &u, &v, &cost);
        push_back(&graph[u], v, cost);
        push_back(&graph[v], u, cost);
        total_cost += cost; // add cost of every edg
    }
    
    // find min cost path between every 2 nodes
    for(int i = 1; i <= n; ++i)
        dijkstra(i, n, minCost, graph);
    

    int num_odd_nodes = 0;
    int* odd_nodes = malloc(n * sizeof(int));    
    for(int i = 1; i <= n; ++i)
        if(graph[i].num_nodes % 2)
            odd_nodes[num_odd_nodes++] = i;        

    int min_cost_of_pairs = 1e9;
    int* vis = calloc(n, sizeof(int));

    find_min_pair_cost(odd_nodes, num_odd_nodes, minCost, vis, 0, &min_cost_of_pairs, 0);
    

    printf("Minimum cost path: %d\n", min_cost_of_pairs + total_cost );

    return 0;
}

/*
    Input format:
        n e
        <e lines of the format u v x,
        denoting an edge of cost x between u and v
        >
*/