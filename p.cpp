#include <iostream>
#include <fstream>
#include <algorithm>
int main(int argc, char **argv)
{

    int myints[3][8] = {32,71,12,45,26,80,53,33,32,71,12,45,26,80,53,33,32,71,12,45,26,80,53,33};
    std::sort(myints[1], myints[1] + 8);
    for(int i=0; i<8;i++)
    {
        std::cout << ' ' << myints[1][i];

    }
}
