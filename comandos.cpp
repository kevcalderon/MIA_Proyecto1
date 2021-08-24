#include "encabezados/Comandos.h"
#include "encabezados/DiscoPrincipal.h"
#include "encabezados/Estructuras.h"
#include <iostream>
#include <stdlib.h>
#include <locale>
#include <cstring>
#include <locale>
#include <fstream>
#include <vector>
#include <cstdlib>


// COMANDO PARA EJECUTAR POR QUE SE OLVIDA XD
// g++ main.cpp comandos.cpp -o a
// ./a "comando"
using namespace std;


DiscoPrincipal diskprincipal;

Comandos:: Comandos(){}

void Comandos::comandosTerminal(string comand){
        vector<string> prueba; 

        
        string tk =  comandoPrincipal(comand);
        //convierto el comando principal a minusculas. 
        for(int i=0; i<tk.size();i++){
            tk[i] = tolower(tk[i]);
        }
        comand.erase(0,tk.length()+1); 
        // Obtengo un arreglo de todas opciones de cada comando.
        vector<string> tks = options(comand);
        // Se para como parametro el comando principal y el arreglo de opciones.
        funciones(tk, tks);
}

void Comandos::funciones(string token, vector<string>opciones){
    if (token == "mkdisk"){
        cout << "SE ESTA CREANDO EL DISCO..." << std::endl;
        diskprincipal.mkdisk(opciones);

    }else if(token == "rmdisk"){
        cout<< "SE ESTA ELIMINANDO EL DISCO..." <<endl;

        diskprincipal.rmdisk(opciones);
    }
}


string Comandos::comandoPrincipal(string text)
{
    string tkn = "";
    bool terminar = false;
    for (char &c : text){
        if (terminar)
        {
            if (c == ' ' || c == '-'){
                break;
            }
            tkn += c;
        }
        else if ((c != ' ' && !terminar))
        {
            if (c=='#'){
                tkn=text;
                break;
            }else{
                tkn += c;
                terminar = true;
            }
        }
    }
    return tkn;
}

vector<string> Comandos::options(string text){
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string token = "";
    int estado = 0;
    for(char &c: text){
        if (estado ==0 && c=='-')
        {
            estado = 1;

        }else if(estado==0 && c=='#'){
            continue;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(c=='='){
                    estado = 2;
                }else if(c == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (c=='\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }
                
            }else if(estado == 3){
                if (c=='\"')
                {
                    estado = 4;
                }
            }else if (estado==4 && c=='\"')
            {
                tokens.clear();
                continue;
            }else if (estado ==4 && c==' ')
            {
                estado = 0;
                tokens.push_back(token);
                token = "";
                continue;
            }
            token+=c;
        }
    }
    return tokens;
}


vector<string> Comandos::split(string text, string text_split)
{
    vector<string> cadena;
    if (text.empty())
    {
        return cadena;
    }
    
    int n = text.length();
    char char_array[n + 1];
    strcpy(char_array, text.c_str());
    char* point = strtok(char_array, text_split.c_str());
    while (point!=NULL)
    {
        cadena.push_back(string(point));
        point = strtok(NULL, text_split.c_str());
    }
    return cadena;
}

