#include <stdio.h>
#include<stdlib.h>
#define MAX 100

typedef struct No {
    int vertice;
    struct No*prox;
}No;

typedef struct{
    No*adj[MAX];
    int visitado[MAX];
    int numVertices;
}Grafo;

No*criarNo(int v){
    No*novo=(No*)malloc(sizeof(No));
    if(novo==NULL){
        printf("Erro de Memoria Otario\n");
        exit(1);
    }
    novo->vertice=v;
    novo->prox=NULL;
    return novo;
}
void inicilizarGrafo(Grafo*g,int vertices){
g->numVertices=vertices;
for(int i; i< vertices; i++){
    g->adj[i]=NULL;
    g->visitado[i]=0;
 }
}

void adicionarAresta(Grafo*g,int origem,int destino){
    No*novo=criarNo(destino);
    novo->prox=g->adj[origem];
    g->adj[destino]=novo;
    
    novo=criarNo(origem);
    novo->prox=g->adj[destino];
    g->adj[destino]=novo;
    
}


void dfs(Grafo*g,int v){
    printf("%d",v);
    g->visitado[v]=1;
    
    No*temp=g->adj[v];
    
    while(temp !=NULL){
        int adj=temp->vertice;
        if(!g->visitado[adj]){
            dfs(g,adj);
        }
        temp=temp->prox;
    }
}

int main()
{
    Grafo g;
    int vertices,arestas;
    int origem,destino,inicio;
    
    printf("Numero De Vertices: ");
    scanf("%d",&vertices);
    inicilizarGrafo(&g,vertices);
    printf("Numero De Arestas: ");
    scanf("%d",&arestas);
    for(int i=0;i<arestas; i++ ){
        printf("Aresta %d(Origem Destino) ",i+1);
        scanf("%d%d",&origem , &destino);
        
        
    if(origem>=vertices||destino>=vertices){
        printf("Vertice Invalido Jumento\n");
        i--;
        continue;
    }
    adicionarAresta(&g,origem,destino);
    }
    printf("Vertice Inicial para Dfs: ");
    scanf("%d",&inicio);
    
    if(inicio>=vertices){
        printf("Vertice Inicial Invalido Jumento \n");
        return 1;
    }
    printf("Resultado Da Dfs:\n");
    dfs(&g,inicio);
    printf("\n");
    return 0;
}
