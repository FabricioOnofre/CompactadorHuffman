#ifndef COMPACTADOR
#define COMPACTADOR

#include "../Nodo/Nodo.h"
#include "../Fila/Fila.h"
#include "../Item/Item.h"
#include <iostream>

using namespace std;

class Compactador
{
    private:
        Fila* fila;
        Nodo* raiz;
        unsigned int *frequencias;
        char** tabelaDeCaminhos;

        void montarArvore();
        void encontrarFrequencias(FILE*);
        void alocaTabela();
        void gerarNovoArq(FILE*, FILE*);
        int geraCodigo(Nodo *, unsigned char, char *, int);

    public:
        Compactador();
        ~Compactador();
        void Compactar();
};

#endif