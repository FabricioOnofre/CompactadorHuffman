#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// Nodo de árvore
typedef struct node
{
    unsigned char byte;
    int frq;
    struct node *left;
    struct node *right;
} node;

// Item de Fila de Prioridade
typedef struct item
{
    node *nodo;
    struct item *prox;
} item;

// Fila de Prioridade
typedef struct Fila
{
    item *primeiro;
} Fila;

// Inicializa a Fila de Prioridade
Fila *criaFila()
{
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->primeiro = NULL;

    return fila;
}

// Inicializa um novo nó de árvore com os valores passados
node *criaNode(unsigned char byte, int freq)
{
    node *novo = (node *)malloc(sizeof(node));

    novo->byte = byte;
    novo->frq = freq;
    novo->left = NULL;
    novo->right = NULL;
}

// insere um item na fila de prioridade
item *insira(node *nodo, Fila *lista)
{
    // Se a Fila de Prioridade não existe, crio-a
    if (lista->primeiro == NULL)
    {
        // Inicializa um novo nodo
        item *novo = (item *)malloc(sizeof(item));

        novo->nodo = nodo;
        novo->prox = NULL;

        lista->primeiro = novo;

        return lista->primeiro;
    }

    // Se o  novo item da fila deve ficar no inicio
    if (nodo->frq < lista->primeiro->nodo->frq)
    {
        item *novo = (item *)malloc(sizeof(item));

        novo->nodo = nodo;
        novo->prox = lista->primeiro;

        // Seta o primeiro da lista como o novo nodo
        lista->primeiro = novo;

        return lista->primeiro;
    }

    else // A posição adequada para inserção é procurada na Fila de Prioridade
    {
        Fila *listaTmp = criaFila();
        listaTmp->primeiro = lista->primeiro->prox;

        lista->primeiro->prox = insira(nodo, listaTmp);

        free(listaTmp);
        return lista->primeiro;
    }
}

// Retorna a quantidae de item na Fila de Prioridade
int tamanho(Fila *lista)
{
    item *inicio = lista->primeiro;
    if (inicio == NULL)
    {
        return 0;
    }

    item *aux = inicio;
    int retorno = 1;

    while (aux->prox != NULL)
    {
        retorno++;
        aux = aux->prox;
    }

    return retorno;
}

// A Fila de Prioridade é montada, sendo que cada item de fila recebe um nodo de árvore
Fila *montarFila(unsigned int *frequencias)
{
    Fila *fila = criaFila();
    for (int i = 0; i < 256; i++)
    {
        if (frequencias[i] != 0)
        {
            node *novo = criaNode(i, frequencias[i]);
            insira(novo, fila);
        }
    }

    return fila;
}

// É retirado o primeiro item da Fila de Prioridade
node *desenfileirar(Fila *lista)
{

    item **inicio = &lista->primeiro;
    if (*inicio == NULL)
        return NULL;

    item *tmp = *inicio;
    node *result = tmp->nodo;
    *inicio = (*inicio)->prox;
    free(tmp); // libera essa posição da memória

    return result;
}

// Seta o valor do nó esquerdo e direito dessa raiz de árvore
void setFilhos(node *raiz, node *esquerda, node *direita)
{
    if (raiz == NULL)
        return;

    raiz->left = esquerda;
    raiz->right = direita;

    if (esquerda == NULL && direita == NULL)
        return;

    if (esquerda == NULL)
    {
        raiz->frq = direita->frq;
        return;
    }

    if (direita == NULL)
    {
        raiz->frq = esquerda->frq;
        return;
    }

    raiz->frq = esquerda->frq + direita->frq;
    return;
}

// A ávore binária é montada, através da junção dos item da Fila de Prioridade
node *montarArvore(Fila *lista)
{
    while (tamanho(lista) >= 2)
    {
        node *esquerda = desenfileirar(lista);
        node *direita = desenfileirar(lista);

        node *novo = criaNode('\0', esquerda->frq + direita->frq);
        setFilhos(novo, esquerda, direita);

        insira(novo, lista);
    }

    return desenfileirar(lista);
}

// Retorna a altura da árvore binária
int altura(node *raiz)
{
    int alturaEsquerda, alturaDireita;

    if (raiz == NULL)
        return 0;

    alturaEsquerda = altura(raiz->left);
    alturaDireita = altura(raiz->right);

    if (alturaEsquerda >= alturaDireita)
        return 1 + alturaEsquerda;

    return 1 + alturaDireita;
}

// Preenche a tabela de frequências de acordo com a aparição de cada byte no arquivo
void encontrarFrequencias(FILE *arquivo, unsigned int *frequencias)
{

    // Verifica se o arquivo é valido
    if (arquivo == NULL)
    {
        printf("Arquivo invalido!\n");
        return;
    }

    // Garante que todos os valores estao setados como 0
    memset(frequencias, 0, 256 * sizeof(unsigned int));

    unsigned char byte;
    fseek(arquivo, 0, SEEK_SET);
    while (fread(&byte, sizeof(char), 1, arquivo))
        frequencias[byte]++;
}

// Inicializa a tablea com o tamanho 'colunas'
// e todos os valores zerados
char **alocaTabela(int colunas)
{
    // Inicializa a tabela
    char **tabela;
    int pos;
    tabela = (char **)malloc(sizeof(char *) * 256);
    int col = colunas + 1;

    for (pos = 0; pos < 256; pos++)
    {
        tabela[pos] = (char *)malloc(colunas * sizeof(char));
        memset(tabela[pos], 0, sizeof(tabela[pos]));
    }

    return tabela;
}

// Gera o caminho até a folha da árvore binária que contém o byte procurado
int geraCodigo(node *noArvore, unsigned char byteProc, char *resultado, int tamanho)
{
    // Caso base da recursão:
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

    if (noArvore->left == NULL && noArvore->right == NULL && noArvore->byte == byteProc)
    {
        resultado[tamanho] = '\0';
        return 1;
    }
    else
    {
        int achou = 0;
        if (noArvore->left != NULL)
        {
            resultado[tamanho] = '0';
            achou = geraCodigo(noArvore->left, byteProc, resultado, tamanho + 1);
        }
        if (achou == 0 && noArvore->right != NULL)
        {
            resultado[tamanho] = '1';
            achou = geraCodigo(noArvore->right, byteProc, resultado, tamanho + 1);
        }
        if (achou == 0)
        {
            resultado[tamanho] = '\0';
        }

        return achou;
    }
}

// Retona se o bit verificado dessa posição do byte atual é 1 ou 0
unsigned char getBit(unsigned char byte, int pos)
{
    unsigned char mascara = (0b00000001 << pos);
    return (byte & mascara) >> pos;
}

// Apaga a árvore binária e libera suas posições de memória
void apagarFolhas(node *raiz)
{
    if (raiz->left == NULL && raiz->right == NULL)
    {
        free(raiz);
    }
    else
    {
        apagarFolhas(raiz->left);
        apagarFolhas(raiz->right);
    }
}

// Apaga a Fila de Prioridade e libera sua posição da memória
void apagarFila(Fila *lista)
{
    free(lista);
}

// Exibe todas as folhas da árvore binária
void imprimirArvore(node *raiz, int tam)
{

    if (raiz->left == NULL && raiz->right == NULL)
    {
        printf("Folha: %c Altura: %d\n", raiz->byte, tam);
        fflush(stdout);
    }
    else
    {
        imprimirArvore(raiz->left, tam + 1);
        imprimirArvore(raiz->right, tam + 1);
    }
}

// Printa o código binário desse caracter "n"
void printaBin(unsigned n)
{
    unsigned i;
    for (i = 1 << 7; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");

    fflush(stdout);
}

// Exibe todos os itens da Fila de prioridade
void printaFila(Fila *lista)
{
    item *tmp = lista->primeiro;
    while (tmp != NULL)
    {
        printf("CHAR: %c ---- FREQ: %u --- BYTE: %d;\n", tmp->nodo->byte, tmp->nodo->frq, tmp->nodo->byte, tmp->nodo->byte);
        tmp = tmp->prox;
    }
    fflush(stdout);
}

// Exibe todos os itens da Tabela que contém um caminho na árvore binária
void printaTabela(char **tabela)
{
    for (int i = 0; i < 256; i++)
        if (strlen(tabela[i]) > 0)
            printf("%d: %s\n", i, tabela[i]);

    fflush(stdout);
}

float findSize(FILE *arqOriginal, int compactador)
{

    fseek(arqOriginal, 0L, SEEK_END);

    // calculating the size of the file
    long int qtdByt = ftell(arqOriginal);
    float qtdByte = (float)qtdByt;

    float resultado = 0;
    char medida[] = "Bytes";
    // Byte
    if (qtdByte < 1024.0f)
    {
        resultado = qtdByte;
        char medida2[] = " Bytes";
        strcpy(medida, medida2);
    }
    // KB
    else if (qtdByte < 1048576.0f)
    {
        resultado = qtdByte / 1024.0f;
        char medida2[] = " KB";
        strcpy(medida, medida2);
    }
    // MB
    else if (qtdByte < 1073741824.0f)
    {
        resultado = qtdByte / 1048576.0f;
        char medida2[] = " MB";
        strcpy(medida, medida2);
    }
    // GB
    else
    {
        resultado = qtdByte / 1073741824.0f;
        char medida2[] = " GB";
        strcpy(medida, medida2);
    }

    if (compactador)
    {
        printf("Tamanho do arquivo COMPACTADO: %4.2f%s \n", resultado, medida);
    }
    else
    {
        printf("Tamanho do arquivo DESCOMPACTADO: %4.2f%s \n", resultado, medida);
    }

    return resultado;
}

// Exibe a Tabela de Frequência do arquivo
void printarFrequencias(unsigned int *frequencias)
{
    for (int pos = 0; pos < 256; pos++)
    {
        printf("Posicao: %c --- Frq: %d\n", pos, frequencias[pos]);
    }
    fflush(stdout);
}

void esperar()
{
    printf("\nPressione ENTER para continuar. ");
    fflush(stdout);
    getchar();
    getchar();
}