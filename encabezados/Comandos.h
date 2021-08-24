#include <iostream>
#include <vector>
using namespace std;


class Comandos{
    public:
        Comandos();
        void comandosTerminal(string comand);
        vector<string> split(string text, string text_split);
        string aMinuscula(string texto);
        void funciones(string principalToken, vector<string>optiones);
        string comandoPrincipal(string text);
        vector<string> options(string text);
};