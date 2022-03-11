#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "Nodo.h"

Nodo::Nodo(unsigned char byte, int frq): left(NULL), right(NULL) {
    this->setByte(byte);
    this->setFrq(frq);
}

void Nodo::setByte(unsigned char byte) {
    this->byte = byte;
}

void Nodo::setFrq(int frq) {
    this->frq = frq;
}

void Nodo::setLeft(Nodo* left) {
    this->left = left;
}

void Nodo::setRight(Nodo* right) {
    this->right = right;
}

unsigned char Nodo::getByte() {
    return this->byte;
}

int Nodo::getFrq() {
    return this->frq;
}

Nodo* Nodo::getLeft() {
    return this->left;
}

Nodo* Nodo::getRight() {
    return this->right;
}

int Nodo::altura() {
    return alturaRecursivo(this);
}

int alturaRecursivo(Nodo* atual) {
    if (atual == NULL)
        return 0;

    int alturaEsquerda = alturaRecursivo(atual->getLeft());
    int alturaDireita = alturaRecursivo(atual->getRight());

    if (alturaEsquerda >= alturaDireita)
        return 1 + alturaEsquerda;

    return 1 + alturaDireita;
}

ostream& operator<<(ostream& s, const Nodo& n) {
    return s << Nodo::toString(n, 0);
}

string Nodo::toString(Nodo n, int altura) {
    std::stringstream ss; 
    if (n.getLeft() == NULL && n.getRight() == NULL)
    {
        ss << "Folha: " << n.getByte() << "Freq: " << n.getFrq() << "Altura: " << altura << "\n";
        return ss.str();
    }
    else
    {
        ss << Nodo::toString(*(n.getLeft()), altura + 1)
          << Nodo::toString(*(n.getRight()), altura + 1);
        return ss.str();
    } 
}
