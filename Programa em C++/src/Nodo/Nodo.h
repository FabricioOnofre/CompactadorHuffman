#ifndef NODO
#define NODO

#include <iostream>

using namespace std;

class Nodo
{
    private:
        unsigned char byte;
        int frq;
        Nodo* left;
        Nodo* right;
        int alturaRecursivo(Nodo*);
        static string toString(Nodo, int);

    public:
        Nodo(unsigned char, int);
        Nodo(const Nodo&);
        ~Nodo();
        int altura();

        void setByte(unsigned char);
        void setFrq(int);
        void setLeft(Nodo*);
        void setRight(Nodo*);

        unsigned char getByte();
        int getFrq();
        Nodo* getLeft();
        Nodo* getRight();

        friend ostream& operator<<(ostream&, const Nodo&);
        friend istream& operator>>(istream&, const Nodo&);
};

#endif