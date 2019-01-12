#include <iostream>
using namespace std;
int fun()
{
    static int cnt = 0; //函数结束后仍然存在
    cnt++;
    cout << cnt << endl;
}
int main()
{
    for (int i = 1; i <= 10; i++)
        fun();
    return 0;
}