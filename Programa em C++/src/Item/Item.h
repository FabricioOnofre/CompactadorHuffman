#ifndef ITEM
#define ITEM

#include "../Nodo/Nodo.h"
#include <iostream>

using namespace std;

class Item
{
    private:
        Nodo* no;
        Item* prox;
        static string toString(Item);

    public:
        Item();
        Item(Nodo*, Item*);
        ~Item();

        Nodo* getNo();
        Item* getProx();
        void setNo(Nodo*);
        void setProx(Item*);

        friend ostream& operator<<(ostream&, const Item&);
        friend istream& operator>>(istream&, const Item&);
};

#endif