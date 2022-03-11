#ifndef FILA
#define FILA

#include "../Nodo/Nodo.h"
#include "../Item/Item.h"
#include <iostream>

using namespace std;

class Fila
{
    private:
        Item* inicio;
        Item* inserir(Item*, Nodo*);

    public:
        Fila();
        Fila(unsigned int*);
        ~Fila();
        void insira(Nodo*); 
        Nodo* desenfileirar();
        int tamanho();

        friend ostream& operator<<(ostream&, const Fila&);
        friend istream& operator>>(istream&, const Fila&);
};

#endif