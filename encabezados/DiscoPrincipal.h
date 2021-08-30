#include <iostream>
#include <vector>
using namespace std;

class DiscoPrincipal{
    public:
        DiscoPrincipal();
        void mkdisk(vector<string> tokens);
        void imprimirDisco(string path);
        void rmdisk(vector<string> tokens);
        void fdisk(vector<string> tokens);
        vector<string> split(string text, string text_split);

};