#include <string>
#include <ctime>

using namespace std;



typedef struct Particion{
    char status = '0'; // 0 no usado. 1 ya esta en uso
    char type = 'p';  // P, E
    char fit= 'w'; // configuracion B, F, W
    int part_start = -1; //
    int size = 0; //tamanio
    char name[16] = {'-'}; //nombre maximo de 16 char
}Particion;

typedef struct MBR{
    int size;
    char fecha[20];
    int signature;
    char fit[1];
    Particion partitions[4];
} MBR;


typedef struct EBR  {
        char part_status = '0';
        char part_fit; //b, f, w
        int part_start;
        int part_size = 0; 
        int part_next = -1;
        char part_name[16];
} EBR;



