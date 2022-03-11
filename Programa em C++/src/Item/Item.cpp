#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "Item.h"

Item::Item(): no(NULL), prox(NULL)
{}

Item::Item(Nodo* nodo, Item* item): no(nodo), prox(item)
{}

string Item::toString(Item item) {
    if (item.getNo() == NULL) {
        return "";
    }

    std::stringstream ss;
    ss  << "Byte: " << item.getNo()->getByte() << " Freq: "
            << item.getNo()->getFrq() << "\n";

    if (item.getProx() != NULL) {
        ss << Item::toString(*(item.getProx()));
    }

    return ss.str();
}

Nodo* Item::getNo() {
    return this->no;
}

Item* Item::getProx() {
    return this->prox;
}

void Item::setNo(Nodo* nodo) {
    this->no = nodo;
}

void Item::setProx(Item* item) {
    this->prox = item;
}

ostream& operator<<(ostream& os, const Item& item) {
    return os << Item::toString(item);
}