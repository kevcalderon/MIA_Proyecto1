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
#include <ctime>

using namespace std;

DiscoPrincipal::DiscoPrincipal(){}

void DiscoPrincipal::mkdisk(vector<string> tokens){
    
    // instancia disco y particiones
    Estructuras::DiscoCommand disco;
    Estructuras::MBR mbr_disco;
    Estructuras::EBR ebr_disco;
    

    string size = "";
    string u = "";
    string path = "";
  
    int tamanio;
    // todos los parametros vienen con su valor
    for (string token :tokens){
        // separa cada parametro hasta encontrar el signo igual.
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
       
        //tk es el parametro y se pasa todo en minusculas
        //token es el valor del parametro

        for(int i=0; i<tk.size();i++){
            tk[i] = tolower(tk[i]);
        }
        //ASIGNACION DE VARIABLES DISCO
        if(tk == "size" ){
            // cout<< "size: " <<token<<endl;
            if (stoi(token) == 0){
                cout<< "El tamanio del disco no puede ser 0." <<endl; 
                break;
            }else{
                // cout<< "tamanio: "<< stoi(token)<<endl; 
                disco.size = stoi(token);
            }
        } else if(tk == "f" ){
            if (token == "f" || token == "b" || token == "w"){
                disco.fit = token.c_str()[0];
                cout<<"disco fit: " << disco.fit<< endl;
                
            }else{
                cout<<"El fit tiene valor invalido."<<endl;
                break;
            }
        } else if(tk == "u"){
         
            if(token == "m" || token == "k"){
                cout<<"Unit valido"<< endl;
                disco.unit = token.c_str()[0];
                
            } else{
                cout<<"El unit tiene un valor invalido."<<endl;
                break;
            }
        } else if(tk == "path"){
            // cout<< "path: " << token <<endl;
            disco.path = token;
        } else{
           cout<< "El parametro " << tk << " no es correcto."<<endl;
        }
    }

//    VALIDACION DE PARAMS OBLIGATORIOS.
    if(disco.size == 0 || disco.path == ""){
        cout<< "El disco no se creo por falta de parametros obligatorios." <<endl;
    } else{
        char buffer[1024];

        //MBR
        mbr_disco.fit =  disco.fit; //fit

        std::time_t t = std::time(0);   
        std::tm* now = std::localtime(&t);  
        string fecha =  to_string(now->tm_mday) + '/' + 
                        to_string(now->tm_mon + 1) + '/' + 
                        to_string(now->tm_year + 1900) + " " + 
                        to_string(now->tm_hour) + ":" + 
                        to_string(now->tm_min) + ":" + 
                        to_string(now->tm_sec);

        mbr_disco.fecha = fecha; // fecha
        mbr_disco.signature = rand()%51;; //id unico,numero 0-50
        if(disco.unit == 'k'){ // tamanio
            // cout<< "tamanio del disco" << disco.size<<endl;
            mbr_disco.size = (disco.size) * 1024;
            // cout<< "mbr del disco" << mbr_disco.size<<endl;
        }else{
            mbr_disco.size = (disco.size) * (1024*1024);
            // cout<< "mbr del disco" << mbr_disco.size<<endl;
        }

        //Particiones
        Estructuras::Particion particionesVacias;
        for (int i=0; i<4; i++){
            mbr_disco.partitions[i] = particionesVacias;
        }
                
    
        FILE *discofile = fopen(disco.path.c_str(), "wb");
        if (discofile != NULL){
            
            for (int i = 0; i < 1024; i++){
                buffer[i] = '\0';
            }   
            
            
            for (int i = 0;i < mbr_disco.size; i++){
                fwrite(&buffer, 1024, 1, discofile);
            }
            
            
            fseek(discofile, 0, SEEK_SET);
            fwrite(&mbr_disco, sizeof(Estructuras::MBR), 1, discofile);
            
            fclose(discofile);
        } else{
            cout << "ERROR, NO SE CREO EL DISK " << endl;
        }

        imprimirDisco(disco.path);
        
    }
    
}

void DiscoPrincipal::rmdisk(vector<string> tokens){
    string path = "";

    for (string param : tokens){

        string tk = param.substr(0, param.find("="));
        param.erase(0,tk.length()+1);
       
        if(tk == "path"){
            path = param;
        } else{
            cout<< "Parametro no permitido "<<endl;
        }
    }

    if(path == ""){
        cout<< "Parametro path es obligatorio para borrar disco."<<endl;
    }else{
        FILE *disko = fopen(path.c_str(), "rb");
        if (disko != NULL){
            char respuesta;
            int eliminar;
            cout<<"Estas seguro de eliminar el disco? s/n"<<endl;
            cin>>respuesta;
            switch (respuesta)
            {
            case 's':
                eliminar = remove(path.c_str());
                cout<<"" <<endl;
                cout<< "El disco se ha eliminado con exito." <<endl;
                break;
            case 'n':
                cout<<""<<endl;
                cout<<"No se ha borrado el disco."<<endl;
                
                break;
            default:
                cout<<"Ninguna de las opciones es permitida... se cancela la eliminacion."<<endl;
                break;
            }

            if(eliminar != 0){
                cout<<""<<endl;
                cout<<"No fue posible eliminar el disco."<<endl;
            }
        }else{
            cout<<""<<endl;
            cout<<"El disco ha eliminar no se encuentra"<<endl;
        }
    }

}


void DiscoPrincipal::imprimirDisco(string path) {
  // ABRIR ARCHIVO
  Estructuras::MBR mbr_data;
  FILE *disk_file = fopen(path.c_str(), "rb+");

  // EXISTE
  if (disk_file != NULL) {
    
    fseek(disk_file, 0, SEEK_SET);
    fread(&mbr_data, sizeof(Estructuras::MBR), 1, disk_file);

    // IMPRIMIR
    cout << "--- MBR ---\n";
    cout << "MBRSize: " << sizeof(Estructuras::MBR) << "\nDate: " << mbr_data.fecha
         << "\nFit: " << mbr_data.fit << "\nSignature: " << mbr_data.signature
         << "\nSize: " << mbr_data.size << "\n";
    cout << "\n--- PARTICIONES ---";
    for (int i = 0; i < 4; i++) {
      cout << "\nName: " << mbr_data.partitions[i].name
           << "\nSize: " << mbr_data.partitions[i].size
           << "\nFit: " << mbr_data.partitions[i].fit
           << "\nType: " << mbr_data.partitions[i].type
           << "\nStart: " << mbr_data.partitions[i].part_start
           << "\nStatus: " << mbr_data.partitions[i].status << '\n';
      cout << "-----";
    }
  } else{
      cout<< "El disco no se encuentra. " <<endl;
  }

  // CERRAR
  fclose(disk_file);
}