#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "Fila.h"

Fila::Fila() {
    this->inicio = NULL; 
}

Fila::Fila(unsigned int* frequencias) {

    this->inicio = NULL; 
    for (unsigned char i = 0; i < 256; i++)
    {
        if (frequencias[i] != 0)
        {
            Nodo *novo = new Nodo(i, frequencias[i]);
            insira(novo);
        }
    }
}


Item* Fila::inserir(Item* item, Nodo *node){


    if (item == NULL)
    {
        // Inicializa um novo nodo
        item = new Item(node, NULL);

        return item;
    }

    // Se o  novo item da fila deve ficar no inicio
    if (node->getFrq() < (item->getNo()->getFrq()))
    {
        // Seta o primeiro da lista como o novo nodo
        item = new Item(node, this->inicio);

        return item;
    }

    else // A posição adequada para inserção é procurada na Fila de Prioridade
    {
        item->setProx(inserir(item->getProx(), node));

        return item;
    }
}

void Fila::insira(Nodo *node)
{
    Fila::inserir(this->inicio, node);
}
    
Nodo* Fila::desenfileirar() {

    Item** inicio = &this->inicio;
    if (*inicio == NULL)
        return NULL;

    Item* tmp = *inicio;
    Nodo* result = tmp->getNo();
    *inicio = (*inicio)->getProx();

    return result;
}

int Fila::tamanho() {
    if (this->inicio == NULL)
        return 0;

    Item* aux = this->inicio;
    int retorno = 1;
    while (aux->getProx() != NULL)
    {
        retorno++;
        aux = aux->getProx();
    }

    return retorno;
}
