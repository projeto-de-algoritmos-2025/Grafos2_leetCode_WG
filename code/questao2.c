typedef struct {
    int *itens;
    int frente;
    int tras;
    int capacidade;
} Deque;

static void dequeInit(Deque *d, int capacidade) {
    if (capacidade < 8) capacidade = 8;
    d->itens = (int*)malloc(sizeof(int) * capacidade);
    d->frente = 0;
    d->tras = 0;
    d->capacidade = capacidade;
}
static int dequeVazio(Deque *d) { return d->frente == d->tras; }
static int dequeProx(Deque *d, int i) { return (i + 1) % d->capacidade; }
static int dequeAnt(Deque *d, int i) { return (i - 1 + d->capacidade) % d->capacidade; }
static void dequeAumentar(Deque *d) {
    int nova = d->capacidade * 2;
    int *novo = (int*)malloc(sizeof(int) * nova);
    int k = 0;
    for (int i = d->frente; i != d->tras; i = (i + 1) % d->capacidade) novo[k++] = d->itens[i];
    free(d->itens);
    d->itens = novo;
    d->frente = 0;
    d->tras = k;
    d->capacidade = nova;
}
static void dequePushFrente(Deque *d, int v) {
    int antes = dequeAnt(d, d->frente);
    if (antes == d->tras) { dequeAumentar(d); antes = dequeAnt(d, d->frente); }
    d->frente = antes;
    d->itens[d->frente] = v;
}
static void dequePushTras(Deque *d, int v) {
    int prox = dequeProx(d, d->tras);
    if (prox == d->frente) { dequeAumentar(d); prox = dequeProx(d, d->tras); }
    d->itens[d->tras] = v;
    d->tras = prox;
}
static int dequePopFrente(Deque *d) {
    int v = d->itens[d->frente];
    d->frente = dequeProx(d, d->frente);
    return v;
}

int minimumObstacles(int** grid, int gridSize, int* gridColSize) {
    int linhas = gridSize;
    int colunas = gridColSize[0];
    int total = linhas * colunas;
    int *dist = (int*)malloc(sizeof(int) * total);
    for (int i = 0; i < total; ++i) dist[i] = INT_MAX / 4;
    dist[0] = 0;

    Deque dq; dequeInit(&dq, total + 5);
    dequePushFrente(&dq, 0);

    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    while (!dequeVazio(&dq)) {
        int u = dequePopFrente(&dq);
        int ux = u / colunas, uy = u % colunas;
        for (int k = 0; k < 4; ++k) {
            int vx = ux + dx[k], vy = uy + dy[k];
            if (vx < 0 || vy < 0 || vx >= linhas || vy >= colunas) continue;
            int v = vx * colunas + vy;
            int custo = grid[vx][vy];
            int nd = dist[u] + custo;
            if (nd < dist[v]) {
                dist[v] = nd;
                if (custo == 0) dequePushFrente(&dq, v);
                else dequePushTras(&dq, v);
            }
        }
    }

    int resposta = dist[total - 1];
    free(dist);
    free(dq.itens);
    return resposta;
}