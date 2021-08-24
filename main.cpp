//va buscarlo en compilador <> y el "" lo busca en la carpeta del proyecto.
#include <iostream>
#include "encabezados/Comandos.h"


using namespace std;


int main(int argc, char *argv[]){
    // Comandos comands;
    // comands.menu();
    //argc : obtener el numero de argumento totales pasados en la en la linea de comandos.
    //argv : es un array con los argumentos pasados en la linea de comandos.

    // cout << "Numero de argumentos pasados: " << argc <<endl;
    // cout << "Lista de argumentos: " <<endl;

    string static lista = "";
    for (int i=1; i<argc; i++){
        lista += argv[i];
        lista += " ";
    } 

    // lista es para analizador.

    // cout << "lista: " << lista << endl;
    Comandos comands;
    comands.comandosTerminal(lista);
    return 0;
}