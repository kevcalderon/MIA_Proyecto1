#include "encabezados/DiscoPrincipal.h"
#include "encabezados/Estructuras.h"
#include "encabezados/Comandos.h"
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
    int size = 0;
    char unit='m';
    string path = "";
    string fit ="f";
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
                cout<< "tamanio: "<< stoi(token)<<endl; 
                size = stoi(token);
            }
        } else if(tk == "f" ){
            if (token == "f" || token == "b" || token == "w"){
                fit = token;
                cout<<"disco fit: " << fit<< endl;
                
            }else{
                cout<<"El fit tiene valor invalido."<<endl;
                break;
            }
        } else if(tk == "u"){
         
            if(token == "m" || token == "k"){
                cout<<"Unit valido"<< endl;
                unit = token.c_str()[0];
                
            } else{
                cout<<"El unit tiene un valor invalido."<<endl;
                break;
            }
        } else if(tk == "path"){
            // cout<< "path: " << token <<endl;
            path = token;
        } else{
           cout<< "El parametro " << tk << " no es correcto."<<endl;
        }
    }

    //VALIDACION DE PARAMS OBLIGATORIOS.
    if(size == 0 || path == ""){
        cout<< "El disco no se creo por falta de parametros obligatorios." <<endl;
    } else{
        // instancia disco y particiones
        MBR mbr_disco;
        EBR ebr_disco;
        
        char buffer[1024];

        std::time_t t = std::time(0);   
        std::tm* now = std::localtime(&t);  
        string fecha =  to_string(now->tm_mday) + '/' + 
                        to_string(now->tm_mon + 1) + '/' + 
                        to_string(now->tm_year + 1900) + " " + 
                        to_string(now->tm_hour) + ":" + 
                        to_string(now->tm_min) + ":" + 
                        to_string(now->tm_sec);

        strcpy(mbr_disco.fecha, fecha.c_str());

        strcpy(mbr_disco.fit, fit.c_str());

        mbr_disco.signature = rand() % 51; //id unico,numero 0-50
        if(unit == 'k'){ // tamanio
            // cout<< "tamanio del disco" << disco.size<<endl;
            mbr_disco.size = (size) * 1024;
            // cout<< "mbr del disco" << mbr_disco.size<<endl;
        }else{
            mbr_disco.size = (size) * (1024*1024);
            // cout<< "mbr del disco" << mbr_disco.size<<endl;
        }
        //Particiones vacias
        Particion particionesVacias;
        for (int i=0; i<4; i++){
            mbr_disco.partitions[i].status = '0';
            mbr_disco.partitions[i].size = 0;
            mbr_disco.partitions[i].type = 'p';
            mbr_disco.partitions[i].fit = 'w';
            mbr_disco.partitions[i].part_start = -1;
            strcpy(mbr_disco.partitions[i].name, "-");
        }
                
        FILE *disk_file = fopen(path.c_str(), "wb");
        // AGREGAR AL DISCO
        if (disk_file != NULL) {
            // GUARDAR MBR
            for (int i = 0; i < 1024; i++)
                buffer[i] = '\0';

            // RELLENAR EL RESTO CON 0
            for (int byte_index = 0;
                byte_index < size;
                byte_index++)
            fwrite(buffer, 1024, 1, disk_file);

            // GUARDAR MBR
            fseek(disk_file, 0, SEEK_SET);
            fwrite(&mbr_disco, sizeof(MBR), 1, disk_file);
            fclose(disk_file);
        } else{
           // OBTENER LA RUTA PARA CREAR EL DIRECTORIO.
            vector<string> ruta = DiscoPrincipal::split(path, "/");
            string newPath = "/";

            for (int i=0; i< ruta.size()-1; i++){
               newPath += ruta[i] + "/";
            }

           
            //CREO EL DIRECTORIO
            string comando1 = "mkdir -p \"" + newPath + "\"";
            system(comando1.c_str());

            //CREO EL DISCO EN DIRECTORIO.
            FILE *discofile = fopen(path.c_str(), "wb");
            for (int i = 0; i < 1024; i++){
                buffer[i] = '\0';
            }   
            
            for (int i = 0;i < mbr_disco.size; i++){
                fwrite(&buffer, 1024, 1, discofile);
            }
            fseek(discofile, 0, SEEK_SET);
            fwrite(&mbr_disco, sizeof(MBR), 1, discofile);
            fclose(discofile);
        }

        
        imprimirDisco(path);
        
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


void DiscoPrincipal::fdisk(vector<string> tokens){
    string estado;
    string size;
    string path;
  
    //VERIFICAR SI EXISTE DELETE EN PARAMETROS.;
    for(string token : tokens){

        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        //tk es el parametro y se pasa todo en minusculas
        //token es el valor del parametro

        for(int i=0; i<tk.size();i++){
            tk[i] = tolower(tk[i]);
        }

        // OBTENER EL PATH;
        if(tk == "path"){
            path = token;
        }


        // VERIFICA QUE TIPOS DE COMANDOS VIENE.
        if(tk == "delete"){
            estado = "delete";
        }else if(tk == "add"){
            estado = "add";
        } else{
            estado = "normal";
        }

    }

    // MBR discoMBR;
    // FILE *disk_file = fopen(path.c_str(), "rb+");
    //AGREGAR PARTICION YA SEA PRIMARIA/EXTENDIDA O LOGICA Y SUS PROPIEDADES
    //ELIMINA LA PARTICION
    if (estado == "delete"){

    //AGREGAR O QUITAR ESPACIO DE LA PARTICION
    }else if(estado == "add"){

    } else{
        char unit = 'k';
        char type = 'p';
        char fit = 'w';
        int size = 0;
        char name[16];
        // OBTENER PARAMETROS PARA PARTICION
        // size, u, type, f, name
        for(string token : tokens){
            string tk = token.substr(0, token.find("="));
            token.erase(0,tk.length()+1);

            for(int i=0; i<tk.size();i++){
                tk[i] = tolower(tk[i]);
            }

            if(tk == "size"){
                size = stoi(token);
                if(size == 0){
                    cout<< ""<<endl;
                    cout <<"El valor de la particion no puede ser 0."<<endl;
                    break;
                }
            }else if(tk == "u"){
                unit = token.c_str()[0];
            }else if(tk == "type"){
                type = token.c_str()[0];
            }else if(tk == "name"){
                strcpy(name, token.c_str());
            }else if(tk == "f"){
                fit = token.c_str()[0];
            }
        }

        cout<< "VARIABLES PARA PARTICION"<<endl;
        cout<< "size: " << size <<endl;
        cout<< "type: " << type <<endl;
        cout<< "unit: " << unit <<endl;
        cout<< "fit: " << fit <<endl;
        cout<< "name: " << name <<endl;
        cout<< "path: " << path <<endl;
        
        
        // VERIFICAR EL ESTADO,
        cout<< "" <<endl;
        cout<<"IMPRIMIR DISCO EXISTENTE"<<endl;
        imprimirDisco(path);
       

    }

}


void DiscoPrincipal::imprimirDisco(string path) {
  // ABRIR ARCHIVO
  MBR mbr_data;
  FILE *disk_file = fopen(path.c_str(), "rb+");

  // EXISTE
  if (disk_file != NULL) {
    
    fseek(disk_file, 0, SEEK_SET);
    fread(&mbr_data, sizeof(MBR), 1, disk_file);

    // IMPRIMIR
    cout << "--- MBR ---\n";
    cout << "MBRSize: " << sizeof(MBR) << "\nDate: " << mbr_data.fecha
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



vector<string> DiscoPrincipal::split(string text, string text_split)
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
