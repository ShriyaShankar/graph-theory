#include<stdio.h>
#include<stdlib.h>

int min(int a,int b)
{
    return(a<b?a:b);
}

typedef struct node
{
 int val;
 struct node* next;
}NODE;

typedef struct graph
{   
 int v;
 NODE** arr;
}GRAPH;

GRAPH* createGraph(int v)
{
    int i;
    GRAPH* temp =(GRAPH*)malloc(sizeof(GRAPH));
    temp->v=v;
    for(i=0;i<v;i++)
     temp->arr=malloc(sizeof(int*)*v);
    for(i=0;i<v;i++)
     temp->arr[i]=NULL;
    return temp;
}

void addEdge(GRAPH* g,int u,int v)
{
    NODE* temp =(NODE*)malloc(sizeof(NODE));
    temp->val = v;
    temp->next = g->arr[u];
    g->arr[u] = temp;     
}

void apUtil(GRAPH * g,int node,int* visited,int* disc,int* parent,int* low,int* ap)
{
    NODE* temp=NULL;
    static int time=0;
    int children=0;
    temp = g->arr[node];
    visited[node]=1;
    time++;
    disc[node]=low[node]=time;

    while(temp!=NULL)
    {       
       if(!visited[temp->val])
        {
          children++;
          parent[temp->val]=node;
          apUtil(g,temp->val,visited,disc,parent,low,ap);

      low[node]= min(low[node],low[temp->val]);

          if(parent[node]==-1 && children>1)
             ap[node]=1;

      if(parent[node]!=-1 && disc[node]<=low[temp->val])
            ap[node]=1;           
        }    
        else if(temp->val!=parent[node])
        {
            low[node]=min(low[node],disc[temp->val]);
        }
       temp= temp->next;
      }
     //printf("%d",node);
}

void AP(GRAPH* g)
{
    int i;
    int* disc = (int*)malloc(sizeof(int)*g->v);
    int* visited = (int*)malloc(sizeof(int)*g->v);
    int* parent = (int*)malloc(sizeof(int)*g->v);
    int* low = (int*)malloc(sizeof(int)*g->v);
    int* ap = (int*)malloc(sizeof(int)*g->v);
    for(i=0;i<g->v;i++)
    {
        visited[i]=0;
        parent[i]=-1;
        ap[i]=0;
    }
    for(i=0;i<g->v;i++)
    {
        if(visited[i]==0)
        {
            apUtil(g,i,visited,disc,parent,low,ap);
        }
    }
    printf("\n");
    int flag=0;
   printf("Articulation Point(s):\n");
    for(i=0;i<g->v;i++)
    {
       
        if(ap[i]==1)
	{
	  flag=1;
	  printf("%d\n",i);
	}
    }
    if(flag==0)
	printf("\nNone");
}

void main()
{
    
    int size=0,edges=0,i,u,v;
   //printf("Enter size of the graph : ");
    scanf("%d",&size);
   //printf("\nEnter the number of edges : ");
    scanf("%d",&edges);
    GRAPH* g = createGraph(size);
    for(i=0;i<edges;i++)
    {
   //printf("\nEnter the edge %d\n",i);
  	scanf("%d %d",&u,&v);
	addEdge(g,u,v);
    }
    AP(g);
} 
