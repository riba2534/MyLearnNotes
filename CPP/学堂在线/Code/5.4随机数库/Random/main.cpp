#include <iostream>
#include "zyrandom.h"
using namespace std;
int main()
{
    Randomize();
    for (int i = 0; i < 8; i++)
    {
        int t = GenerateRandomNumber(10, 99);
        cout << t << ";";
    }
    cout << endl;
    for (int i = 0; i < 8; i++)
    {
        double t = GenerateRandomReal(10, 99);
        cout << t << ";";
    }
    cout << endl;
    return 0;
}
