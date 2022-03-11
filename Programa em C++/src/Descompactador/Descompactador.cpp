#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include "Descompactador.h"

using namespace std;

Descompactador::Descompactador():
fila(NULL), raiz(NULL),
tabelaDeCaminhos(NULL), frequencias(new unsigned int[256])
{

};

void Descompactador::montarArvore(){
    
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

void Descompactador::alocaTabela(){
    
    // Inicializa a tabela
    const int colunas = this->raiz->altura();
    this->tabelaDeCaminhos = (char**) malloc(sizeof(char*) * 256);

    for (int pos = 0; pos < 256; pos++)
    {
        this->tabelaDeCaminhos[pos] = (char*) malloc(colunas * sizeof(char));
        memset(this->tabelaDeCaminhos[pos], 0 , sizeof(this->tabelaDeCaminhos[pos]));
    }
};

unsigned char Descompactador::getBit(unsigned char byte, int pos){
    unsigned char mascara = (0b00000001 << pos);
    return (byte & mascara) >> pos;
}


void Descompactador::gerarNovoArq(FILE* arqEntrada, FILE* arqSaida)
{

    Nodo *aux = this->raiz;

    unsigned char byte;
    unsigned char proxByte;
    fread(&byte, sizeof(byte), 1, arqEntrada);

    // Le até o ultimo byte do arquivo compactado e escreve
    // seus bytes correspondentes no arquivo final
    while (fread(&proxByte, sizeof(proxByte), 1, arqEntrada))
    {
        // Le cada bit do byte e percorre a arvore de acordo
        for (int pos = 7; pos >= 0; pos--)
        {
            unsigned int bit = getBit(byte, pos);

            if (bit)
                aux = aux->getRight();
            else
                aux = aux->getLeft();

            // Chegou em uma folha
            if (aux->getRight() == NULL && aux->getLeft() == NULL)
            {
                unsigned char valor = aux->getByte();
                fwrite(&valor, sizeof(unsigned char), 1, arqSaida);
                aux = this->raiz;
            }
        }

        byte = proxByte;
    }

    // Le o ultimo byte do arquivo original e aplica a regra do BitsAIgnorar
    for (int pos = 7; pos >= bitsAIgnorar; pos--)
    {
        unsigned int bit = getBit(byte, pos);

        if (bit)
            aux = aux->getRight();
        else
            aux = aux->getLeft();

        if (aux->getRight() == NULL && aux->getLeft() == NULL)
        {
            unsigned char valor = aux->getByte();
            fwrite(&valor, sizeof(unsigned char), 1, arqSaida);
            aux = this->raiz;
        }
    }
};

void Descompactador::Descompactar() {

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
    FILE* arqCompactado     = fopen(endereco, "rb");
    FILE *arqDescompactado  = fopen(endr_arq_comp, "wb");

    // Verifica se o arquivo é válido
    if (arqCompactado == NULL)
    {
        cout << "Arquivo nao encontrado!\n";       
        fclose(arqCompactado);
        fclose(arqDescompactado);
        return;
    }


    fread(&bitsAIgnorar, sizeof(unsigned char), 1, arqCompactado);
    fread(this->frequencias, sizeof(unsigned int), 256, arqCompactado);
    this->fila = new Fila(this->frequencias);
    this->montarArvore();
    this->alocaTabela();
    this->gerarNovoArq(arqCompactado, arqDescompactado);

    fclose(arqCompactado);
    fclose(arqDescompactado);

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

    cout << "\nArquivo compactado com sucesso no diretorio: \n";
    cout << endr_arq_comp;
    cout << "\n\nTempo de compactacao: " << seconds << "\n";
};
