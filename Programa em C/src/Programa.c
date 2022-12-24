/*
    Integrantes do grupo:
    Fabio Alves     - 20129
    Fabricio Onofre - 20130
    Gustavo Mendes  - 20136
    Rafael Dosso    - 20154

    Disciplina: TOO - COTUCA - 2021 - PD21
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#include "Funcoes.c"

void compactador()
{

    char nome[200];
    char endereco[200];

    // Seta a lingua como portugues
    setlocale(LC_ALL, "Portuguese");
    printf("\n*****************************************************************\n");
    printf("\nCOMPACTADOR DE ARQUIVOS - HUFFMAN\n");

    // Le o endereco e o nome do arquivo fonte
    printf("\nDigite o endereco do arquivo a ser compactado: ");
    fflush(stdout);

    scanf("\n\t%s", endereco);
    fflush(stdin);

    printf("Digite o nome do arquivo que contera a compactacao: ");
    fflush(stdout);
    scanf("\n\t%s", nome);
    fflush(stdin);

    printf("\n*****************************************************************\n");

    char endr_arq_comp[200] = "../output-files/";
    strcat(endr_arq_comp, nome);
    strcat(endr_arq_comp, ".huf");

    clock_t start = clock();

    // Inicializa os arquivos
    FILE *arqOriginal = fopen(endereco, "rb");

    // Verifica se o arquivo é válido
    if (arqOriginal == NULL)
    {
        printf("Arquivo nao encontrado!\n");
        return;
    }
    FILE *arqCompactado = fopen(endr_arq_comp, "wb");

    // Faz o levantamento da frequencia de cada byte
    unsigned int frequencias[256];
    encontrarFrequencias(arqOriginal, frequencias);
    // printarFrequencias(frequencias);

    // Monta a fila com base nas frequencias
    Fila *fila = montarFila(frequencias);
    // printaFila(fila);

    // Monta a arvore com a fila
    node *raiz = montarArvore(fila);
    // imprimirArvore(raiz, 0);

    // Escreve o cabecalho de informacoes no arquivo compactado
    unsigned char bitsAIgnorar = 0;
    fwrite(&bitsAIgnorar, sizeof(unsigned char), 1, arqCompactado);
    fwrite(frequencias, sizeof(unsigned int), 256, arqCompactado);

    // Inicializa a tabela de codigos como 0
    char **tabela;
    const int colunas = altura(raiz);
    tabela = alocaTabela(colunas);

    unsigned char tam = 1;
    unsigned char buffer = 0b00000000;

    // Seta o cursor no  comeco novamente
    fseek(arqOriginal, 0, SEEK_SET);

    // Le byte a byte do arquivo
    unsigned char byte;
    while (fread(&byte, 1, sizeof(unsigned char), arqOriginal) >= 1)
    {
        char codigo[colunas];
        memset(codigo, 0, sizeof(codigo));

        // Se o codigo nao esta na tabela, gera-o
        if (strlen(tabela[byte]) == 0)
        {
            geraCodigo(raiz, byte, codigo, 0);

            strcpy(tabela[byte], codigo);
        }
        // Se o codigo ja esta na tabela, usa-o1
        else
        {
            strcpy(codigo, tabela[byte]);
        }

        // Percorre cada char '0' ou '1' de codigo do byte atual
        int pos = 0;
        for (char i = codigo[pos]; i != '\0'; i = codigo[++pos])
        {
            if (i == '1')
                buffer = buffer | (0b00000001 << 8 - tam);
            // if (i == '0')
            //     nao precisa ser feito nada, ja que buffer é
            //     inicializado com apenas 0s

            // Quando formar um byte completo com 8 bits
            if (tam == 8)
            {
                fwrite(&buffer, sizeof(unsigned char), 1, arqCompactado);
                // Zera a variável buffer e o contador de bits
                buffer = 0;
                tam = 0;
            }
            tam++;
        }
    }
    // printaTabela(tabela);

    fwrite(&buffer, 1, sizeof(unsigned char), arqCompactado);

    bitsAIgnorar = 8 - (--tam);
    fseek(arqCompactado, 0, SEEK_SET);
    fwrite(&bitsAIgnorar, sizeof(unsigned char), 1, arqCompactado);

    printf("\n///////////////////////////////////////////////////////////////////\n");
    printf("\nRESULTADOS DA COMPACTACAO\n");
    printf("\nArquivo COMPACTADO no diretorio: %s\n\n", endr_arq_comp);
    float arqOrg = findSize(arqOriginal, 1);
    float arqComp = findSize(arqCompactado, 0);
    printf("\nTaxa de efetividade: %4.2f%% ", 100.0f - ((arqComp / arqOrg) * 100));
    // Fecha os arquivos
    fclose(arqOriginal);
    fclose(arqCompactado);

    apagarFolhas(raiz);
    apagarFila(fila);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de compactacao: %.2lfs\n", seconds);
    printf("\n///////////////////////////////////////////////////////////////////\n");

    fflush(stdout);
}

void descompactador()
{

    char nome[200];
    char endereco[200];

    printf("\n*****************************************************************\n");
    printf("\nDESCOMPACTADOR DE ARQUIVOS - HUFFMAN\n");
    // Le o endereco e o nome do arquivo fonte
    printf("\nDigite o endereco do arquivo a ser descompactado: ");
    fflush(stdout);
    scanf("\n\t%s", endereco);
    fflush(stdin);

    printf("Digite o nome do arquivo que contera a descompactacao: ");
    fflush(stdout);
    scanf("\n\t%s", nome);
    fflush(stdin);

    printf("\n*****************************************************************\n");

    // Inicializa os arquivos
    char caminho_arquivo_descompactado[200] = "../output-files/";
    strcat(caminho_arquivo_descompactado, nome);

    clock_t start = clock();

    FILE *arqCompactado = fopen(endereco, "rb");

    // Verifica se o endereco fornecido é valido
    if (arqCompactado == NULL)
    {
        printf("Arquivo nao encontrado!\n");
        return;
    }
    FILE *arqDescompactado = fopen(caminho_arquivo_descompactado, "wb");
    fseek(arqCompactado, 0, SEEK_SET);

    // Le os cabecalhos
    unsigned char bitsAIgnorar;
    fread(&bitsAIgnorar, sizeof(unsigned char), 1, arqCompactado);
    unsigned int frequencias[256];
    fread(frequencias, sizeof(unsigned int), 256, arqCompactado);

    // Monta a fila com base nas frequencias
    Fila *fila = montarFila(frequencias);

    // Monta a arvore com base na fila
    node *raiz = montarArvore(fila);

    // Inicializa a tabela de codigos como 0
    char **tabela;
    const int colunas = altura(raiz);
    tabela = alocaTabela(colunas);

    node *aux = raiz;

    unsigned char byte;
    unsigned char proxByte;
    fread(&byte, sizeof(byte), 1, arqCompactado);

    // Le até o ultimo byte do arquivo compactado e escreve
    // seus bytes correspondentes no arquivo final
    while (fread(&proxByte, sizeof(proxByte), 1, arqCompactado))
    {
        // Le cada bit do byte e percorre a arvore de acordo
        for (int pos = 7; pos >= 0; pos--)
        {
            unsigned int bit = getBit(byte, pos);

            if (bit)
                aux = aux->right;
            else
                aux = aux->left;

            // Chegou em uma folha
            if (aux->right == NULL && aux->left == NULL)
            {
                fwrite(&aux->byte, sizeof(unsigned char), 1, arqDescompactado);
                aux = raiz;
            }
        }

        byte = proxByte;
    }

    // Le o ultimo byte do arquivo original e aplica a regra do BitsAIgnorar
    for (int pos = 7; pos >= bitsAIgnorar; pos--)
    {
        unsigned int bit = getBit(byte, pos);

        if (bit)
            aux = aux->right;
        else
            aux = aux->left;

        if (aux->right == NULL && aux->left == NULL)
        {
            fwrite(&aux->byte, sizeof(unsigned char), 1, arqDescompactado);
            aux = raiz;
        }
    }


    printf("\n///////////////////////////////////////////////////////////////////\n");
    printf("\nRESULTADOS DA DESCOMPACTACAO\n");
    printf("\nArquivo DESCOMPACTADO no diretorio: %s\n\n", caminho_arquivo_descompactado);
    float arqOrg = findSize(arqCompactado, 1);
    float arqComp = findSize(arqDescompactado, 0);
    //printf("\nTaxa de efetividade: %4.2f%% ", 100.0f - ((arqComp / arqOrg) * 100));
    // Fecha os arquivos
    fclose(arqCompactado);
    fclose(arqDescompactado);

    apagarFolhas(raiz);
    apagarFila(fila);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("\nTempo de descompactacao: %.2lfs\n", seconds);
    printf("\n///////////////////////////////////////////////////////////////////\n");

    fflush(stdout);
}

int main()
{
    int opcao = 0;

    do
    {
        system("cls");
        printf("ALGORITMO DE HUFFMAN - (DES)COMPACTACAO DE ARQUIVOS\n");
        printf("\n------------------------------\n");
        printf("1 - Compactador \n");
        printf("2 - Descompactador\n");
        printf("3 - Fechar Programa \n");
        printf("------------------------------\n");
        printf("Escolha uma opcao: ");
        fflush(stdout);
        scanf("%d", &opcao);
        fflush(stdin);
        printf("\n");

        switch (opcao)
        {
        case 1:
        {
            compactador();
            break;
        }

        case 2:
        {
            descompactador();
            break;
        }
        }

        if (opcao != 3)
        {
            printf("\n");
            fflush(stdout);
            esperar();
        }

    } while (opcao != 3);

    printf("Programa finalizado com exito.\n");
    return 0;
}