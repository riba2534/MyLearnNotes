#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;
void Randomize()
{
    srand((int)time(0));
}
int GenerateRandomNumber(int low, int high) //把比例映射到区间中去
{
    double _d;
    if (low > high)
    {
        cout << "GenerateRandomNumber: Make sure low<=high." << endl;
        exit(1);
    }
    _d = (double)rand() / ((double)RAND_MAX + 1.0);
    return low + (int)(_d * (high - low + 1));
}
double GenerateRandomReal(double low, double high)
{
    double _d;
    if (low > high)
    {
        cout << "GenerateRandomNumber: Make sure low<=high." << endl;
        exit(2);
    }
    _d = (double)rand() / (double)RAND_MAX;
    return low + _d * (high - low);
}
