#include <iostream>
#include <string.h>

#include "Compactador.h"

using namespace std;

Compactador::Compactador():
fila(NULL), raiz(NULL),
tabelaDeCaminhos(NULL), frequencias(new unsigned int[256])
{

};

void Compactador::encontrarFrequencias(FILE* arquivo){
    
    memset(frequencias, 0, 256 * sizeof(unsigned int));

    // Verifica se o arquivo é valido
    if (arquivo == NULL) {
        cout << "Arquivo invalido!\n";
        return;
    }

    unsigned char byte;
    fseek(arquivo, 0, SEEK_SET);
    while (fread(&byte, sizeof(char), 1, arquivo))
        frequencias[byte]++;
};

void Compactador::montarArvore(){
    
    while (this->fila->tamanho() >= 2)
    {
        Nodo *esquerda = this->fila->desenfileirar();
        Nodo *direita  = this->fila->desenfileirar();

        Nodo *novo = new Nodo('\0', esquerda->getFrq() + direita->getFrq()); 
        novo->setLeft(esquerda);
        novo->setRight(direita);
        this->fila->insira(novo);
    }

    this->raiz = this->fila->desenfileirar();
};

void Compactador::alocaTabela(){
    
    // Inicializa a tabela
    const int colunas = this->raiz->altura();
    this->tabelaDeCaminhos = (char**) malloc(sizeof(char*) * 256);

    for (int pos = 0; pos < 256; pos++)
    {
        this->tabelaDeCaminhos[pos] = (char*) malloc(colunas * sizeof(char));
        memset(this->tabelaDeCaminhos[pos], 0 , sizeof(this->tabelaDeCaminhos[pos]));
    }
};

int Compactador::geraCodigo(Nodo *noArvore, unsigned char byteProc, char *resultado, int tamanho)
{

    // Caso base da recursão:
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

    if (noArvore->getLeft() == NULL && noArvore->getRight() == NULL && noArvore->getByte() == byteProc)
    {
        resultado[tamanho] = '\0';
        return 1;
    }
    else
    {
        int achou = 0;
        if (noArvore->getLeft() != NULL)
        {
            resultado[tamanho] = '0';
            achou = geraCodigo(noArvore->getLeft(), byteProc, resultado, tamanho + 1);
        }
        if (achou == 0 && noArvore->getRight() != NULL)
        {
            resultado[tamanho] = '1';
            achou = geraCodigo(noArvore->getRight(), byteProc, resultado, tamanho + 1);
        }
        if (achou == 0)
        {
            resultado[tamanho] = '\0';
        }

        return achou;
    }
};


void Compactador::gerarNovoArq(FILE* arqEntrada, FILE* arqSaida)
{
    fseek(arqSaida, 0, SEEK_SET);
    unsigned char bitsAIgnorar = 0;
    fwrite(&bitsAIgnorar, sizeof(unsigned char), 1, arqSaida);
    fwrite(this->frequencias, sizeof(unsigned int), 256, arqSaida);

    const int colunas = this->raiz->altura();
    unsigned char tam = 1;
    unsigned char buffer = 0b00000000;

    // Seta o cursor no  comeco novamente
    fseek(arqEntrada, 0, SEEK_SET);

    // Le byte a byte do arquivo
    unsigned char byte;
    while (fread(&byte, 1, sizeof(unsigned char), arqEntrada) >= 1)
    {
        char codigo[colunas];
        memset(codigo, 0, sizeof(codigo));

        // Se o codigo nao esta na tabela, gera-o
        if (strlen(this->tabelaDeCaminhos[byte]) == 0)
        {
            geraCodigo(this->raiz, byte, codigo, 0);            
            strcpy(this->tabelaDeCaminhos[byte], codigo);
        }
        // Se o codigo ja esta na tabela, usa-o
        else
        {
            strcpy(codigo, this->tabelaDeCaminhos[byte]);
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
                fwrite(&buffer, sizeof(unsigned char), 1, arqSaida);
                // Zera a variável buffer e o contador de bits
                buffer = 0;
                tam = 0;
            }
            tam++;
        }
    }
    //printaTabela(tabela);

    fwrite(&buffer, 1, sizeof(unsigned char), arqSaida);

    bitsAIgnorar = 8 - (--tam);
    fseek(arqSaida, 0, SEEK_SET);
    fwrite(&bitsAIgnorar, sizeof(unsigned char), 1, arqSaida);
    

};

void Compactador::Compactar() {

    char nome[200];
    char endereco[200];

    // Seta a lingua como portugues 
    setlocale(LC_ALL, "Portuguese");
    cout << "\n\tCompactador de arquivos\n";

    // Le o endereco e o nome do arquivo fonte
    cout << "\nDigite o endereco do arquivo a ser compactado: ";
    cin >> endereco;

    cout << "\nDigite o nome do arquivo que contera a compactacao: ";
    cin >> nome;

    char endr_arq_comp[200] = "../output-files/";
    strcat(endr_arq_comp, nome);
    strcat(endr_arq_comp, ".huf");

    clock_t start = clock();

    // Inicializa os arquivos
    FILE* arqOriginal = fopen(endereco, "rb");
    FILE *arqCompactado = fopen(endr_arq_comp, "wb");

    // Verifica se o arquivo é válido
    if (arqOriginal == NULL)
    {
        cout << "Arquivo nao encontrado!\n";       
        fclose(arqOriginal);
        fclose(arqCompactado);
        return;
    }


    this->encontrarFrequencias(arqOriginal);
    this->fila = new Fila(this->frequencias);
    this->montarArvore();
    this->alocaTabela();
    this->gerarNovoArq(arqOriginal, arqCompactado);

    fclose(arqOriginal);
    fclose(arqCompactado);

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

    cout << "\nArquivo compactado com sucesso no diretorio: \n";
    cout << endr_arq_comp;
    cout << "\n\nTempo de compactacao: " << seconds << "\n";
};
