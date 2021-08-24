#include <string>
#include <ctime>

using namespace std;

class Estructuras {
    public:
        Estructuras();
        typedef struct DiscoCommand{
            char unit = 'm'; //opcional
            char fit = 'f'; //opcional
            string path = "";
            int size = 0;
        } DiscoCommand;

        typedef struct Particion{
            char status = '0'; // 0 no usado. 1 ya esta en uso
            char type;  // P, E
            char fit; // configuracion B, F, W
            int part_start = -1; //
            int size = 0; //
            char name[16]; //nombre maximo de 16 char
        }Particion;

        typedef struct MBR{
            int size;
            string fecha;
            int signature;
            char fit;
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
};
