typedef struct {
    int* id;
    int* rank;
    int* weight;
    int size;
} Union;

Union* unionFindInit(int n) {
    Union* uf = (Union*)malloc(sizeof(Union));
    uf->size = n;
    uf->id = (int*)malloc(n * sizeof(int));
    uf->rank = (int*)calloc(n, sizeof(int)); // Initialize to 0
    uf->weight = (int*)malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        uf->id[i] = i;
        uf->weight[i] = (1 << 17) - 1;
    }
    
    return uf;
}

void unionFindFree(Union* uf) {
    free(uf->id);
    free(uf->rank);
    free(uf->weight);
    free(uf);
}

int find(Union* uf, int u) {
    if (uf->id[u] == u) {
        return u;
    }
    return uf->id[u] = find(uf, uf->id[u]);
}

void unionByRank(Union* uf, int u, int v, int w) {
    int i = find(uf, u);
    int j = find(uf, v);
    int newWeight = uf->weight[i] & uf->weight[j] & w;
    uf->weight[i] = newWeight;
    uf->weight[j] = newWeight;
    
    if (i == j) {
        return;
    }
    
    if (uf->rank[i] < uf->rank[j]) {
        uf->id[i] = j;
    } else if (uf->rank[i] > uf->rank[j]) {
        uf->id[j] = i;
    } else {
        uf->id[i] = j;
        uf->rank[j]++;
    }
}

int getMinCost(Union* uf, int u, int v) {
    if (u == v) {
        return 0;
    }
    int i = find(uf, u);
    int j = find(uf, v);
    return (i == j) ? uf->weight[i] : -1;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* minimumCost(int n, int** edges, int edgesSize, int* edgesColSize, int** query, int querySize, int* queryColSize, int* returnSize) {
    *returnSize = querySize;
    int* result = (int*)malloc(querySize * sizeof(int));
    Union* uf = unionFindInit(n);
    
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        unionByRank(uf, u, v, w);
    }
    
    for (int i = 0; i < querySize; i++) {
        int u = query[i][0];
        int v = query[i][1];
        result[i] = getMinCost(uf, u, v);
    }
    
    unionFindFree(uf);
    return result;
}