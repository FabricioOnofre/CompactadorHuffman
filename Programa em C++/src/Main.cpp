#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "./Compactador/Compactador.h"
#include "./Descompactador/Descompactador.h"

using namespace std;

int main()
{
    int opcao = 0;

    do
    {
        system("cls");
        cout << "Algoritmo de Huffman - Compactacao de arquivos\n"
             << "\n------------------------------\n"
             << "1 - Compactador de arquivos\n"
             << "2 - Descompactador de arquivos \n"
             << "3 - Fechar Programa \n"
             << "------------------------------\n"
             << "Escolha uma opcao: ";

        cin >> opcao;

        cout << "\n";

        Compactador     *compactador    = new Compactador();
        Descompactador  *descompactador = new Descompactador();

        switch (opcao)
        {
            case 1:
            {
                
                cout << "CASE 1\n";
                (*compactador).Compactar();
                
                break;
            }

            case 2:
            {
                cout << "CASE 2\n";                
                (*descompactador).Descompactar();
                break;
            }
        }


        if (opcao != 3)
        {
            cout << "\n";
            cout << "\nPressione ENTER para continuar. ";
            char * teste;
            cin >> teste;
        }

    } while (opcao != 3);

    cout << "Programa finalizado com exito.\n";
    return 0;
}
