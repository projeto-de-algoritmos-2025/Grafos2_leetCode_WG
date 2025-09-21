typedef struct {
    int para;
    int peso;
} Aresta;

typedef struct {
    Aresta *itens;
    int tamanho;
    int capacidade;
} ListaArestas;

static void listaArestasInit(ListaArestas *lista, int capacidade) {
    if (capacidade < 4) capacidade = 4;
    lista->itens = (Aresta*)malloc(sizeof(Aresta) * capacidade);
    lista->tamanho = 0;
    lista->capacidade = capacidade;
}
static void listaArestasPush(ListaArestas *lista, Aresta a) {
    if (lista->tamanho == lista->capacidade) {
        lista->capacidade *= 2;
        lista->itens = (Aresta*)realloc(lista->itens, sizeof(Aresta) * lista->capacidade);
    }
    lista->itens[lista->tamanho++] = a;
}

typedef struct {
    int no;
    int dist;
} ItemFila;

typedef struct {
    ItemFila *dados;   /* 1-indexado */
    int tamanho;
    int capacidade;
} HeapMin;

static void heapTrocar(ItemFila *a, ItemFila *b) { ItemFila t = *a; *a = *b; *b = t; }
static void heapInit(HeapMin *h, int capacidade) {
    if (capacidade < 8) capacidade = 8;
    h->dados = (ItemFila*)malloc(sizeof(ItemFila) * (capacidade + 1));
    h->tamanho = 0;
    h->capacidade = capacidade + 1;
}
static void heapPush(HeapMin *h, ItemFila it) {
    if (h->tamanho + 1 >= h->capacidade) {
        h->capacidade *= 2;
        h->dados = (ItemFila*)realloc(h->dados, sizeof(ItemFila) * h->capacidade);
    }
    int i = ++h->tamanho;
    h->dados[i] = it;
    while (i > 1 && h->dados[i].dist < h->dados[i/2].dist) {
        heapTrocar(&h->dados[i], &h->dados[i/2]);
        i /= 2;
    }
}
static int heapVazio(HeapMin *h) { return h->tamanho == 0; }
static ItemFila heapPop(HeapMin *h) {
    ItemFila topo = h->dados[1];
    h->dados[1] = h->dados[h->tamanho--];
    int i = 1;
    while (1) {
        int l = i*2, r = l+1, m = i;
        if (l <= h->tamanho && h->dados[l].dist < h->dados[m].dist) m = l;
        if (r <= h->tamanho && h->dados[r].dist < h->dados[m].dist) m = r;
        if (m == i) break;
        heapTrocar(&h->dados[i], &h->dados[m]);
        i = m;
    }
    return topo;
}

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {
    (void)timesColSize;

    ListaArestas *grafo = (ListaArestas*)malloc((n + 1) * sizeof(ListaArestas));
    for (int i = 1; i <= n; ++i) listaArestasInit(&grafo[i], 2);
    for (int i = 0; i < timesSize; ++i) {
        int de = times[i][0], para = times[i][1], peso = times[i][2];
        listaArestasPush(&grafo[de], (Aresta){para, peso});
    }

    const int INF = INT_MAX / 4;
    int *distancias = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; ++i) distancias[i] = INF;
    distancias[k] = 0;

    HeapMin fila;
    heapInit(&fila, n + 5);
    heapPush(&fila, (ItemFila){k, 0});

    while (!heapVazio(&fila)) {
        ItemFila atual = heapPop(&fila);
        int no = atual.no, d = atual.dist;
        if (d > distancias[no]) continue;

        for (int i = 0; i < grafo[no].tamanho; ++i) {
            int prox = grafo[no].itens[i].para;
            int w = grafo[no].itens[i].peso;
            if (d + w < distancias[prox]) {
                distancias[prox] = d + w;
                heapPush(&fila, (ItemFila){prox, distancias[prox]});
            }
        }
    }

    int resposta = 0;
    for (int i = 1; i <= n; ++i) {
        if (distancias[i] == INF) { resposta = -1; break; }
        if (distancias[i] > resposta) resposta = distancias[i];
    }

    for (int i = 1; i <= n; ++i) free(grafo[i].itens);
    free(grafo);
    free(distancias);
    free(fila.dados);

    return resposta;
}