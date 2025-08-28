#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// ------------------- KRUSKAL ALGORITMUS - MINIMÁLIS FESZÍTŐFA -------------------

// Union-Find (Disjoint Set) struktúra
int parent[100], rank[100];

int find(int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]);
}

void union_sets(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
        if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

typedef struct {
    int u, v, weight;
} Edge;

Edge edges[10];
int num_edges = 0;

// Kruskal algoritmus implementációja
void kruskal(int num_nodes) {
    int mst_weight = 0;
    for (int i = 0; i < num_nodes; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    // Rendezés élek súlya szerint növekvő sorrendben
    for (int i = 0; i < num_edges - 1; i++) {
        for (int j = 0; j < num_edges - 1 - i; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    // Kruskal algoritmus
    printf("\nKruskal algoritmus: Minimális feszítőfa:\n");
    for (int i = 0; i < num_edges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        if (find(u) != find(v)) {
            union_sets(u, v);
            printf("Él: %d - %d, Súly: %d\n", u, v, edges[i].weight);
            mst_weight += edges[i].weight;
        }
    }
    printf("Minimális feszítőfa összsúlya: %d\n", mst_weight);
}

// ------------------- FLOYD-WARSHALL ALGORITMUS - TRANZITÍV LEZÁRT -------------------

#define INF INT_MAX
#define MAX_NODES 5

void floyd_warshall(int graph[MAX_NODES][MAX_NODES], int num_nodes) {
    int dist[MAX_NODES][MAX_NODES];

    // Inicializáljuk a távolságokat
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (graph[i][j] != 0) {
                dist[i][j] = graph[i][j];
            } else {
                dist[i][j] = INF;
            }
        }
    }

    // Floyd-Warshall algoritmus
    for (int k = 0; k < num_nodes; k++) {
        for (int i = 0; i < num_nodes; i++) {
            for (int j = 0; j < num_nodes; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Eredmények kiírása
    printf("\nFloyd-Warshall algoritmus: Tranzitív lezárás:\n");
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            if (dist[i][j] == INF) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

// ------------------- DIJKSTRA ALGORITMUS - LEGRÖVIDEBB UTAK -------------------

void dijkstra(int graph[MAX_NODES][MAX_NODES], int source, int num_nodes) {
    int dist[MAX_NODES];
    int visited[MAX_NODES] = {0};
    
    // Kezdeti távolságok
    for (int i = 0; i < num_nodes; i++) {
        dist[i] = INF;
    }
    dist[source] = 0;

    for (int i = 0; i < num_nodes - 1; i++) {
        int min = INF, u = -1;
        for (int j = 0; j < num_nodes; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        visited[u] = 1;

        // Frissítjük a távolságokat
        for (int v = 0; v < num_nodes; v++) {
            if (graph[u][v] != 0 && !visited[v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Eredmények kiírása
    printf("\nDijkstra algoritmus: Legrövidebb utak a forrásból (csúcs %d):\n", source);
    for (int i = 0; i < num_nodes; i++) {
        if (dist[i] == INF) {
            printf("Csúcs %d: INF\n", i);
        } else {
            printf("Csúcs %d: %d\n", i, dist[i]);
        }
    }
}

// ------------------- FŐ PROGRAM -------------------

int main() {
    // Kruskal algoritmus példa
    printf("Kruskal algoritmus - Minimális feszítőfa\n");

    edges[0] = (Edge){0, 1, 10};
    edges[1] = (Edge){0, 2, 6};
    edges[2] = (Edge){0, 3, 5};
    edges[3] = (Edge){1, 3, 15};
    edges[4] = (Edge){2, 3, 4};
    num_edges = 5;

    kruskal(MAX_NODES);

    // Floyd-Warshall algoritmus példa
    int graph[MAX_NODES][MAX_NODES] = {
        {0, 10, 0, 5, 0},
        {10, 0, 3, 0, 0},
        {0, 3, 0, 2, 0},
        {5, 0, 2, 0, 3},
        {0, 0, 0, 3, 0}
    };
    floyd_warshall(graph, MAX_NODES);

    // Dijkstra algoritmus példa
    dijkstra(graph, 0, MAX_NODES); // Forrás csúcs 0

    return 0;
}
