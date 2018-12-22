#include <bits/stdc++.h>
using namespace std;
int find_max(int arr[], int n)
{
    int maxx = 0;
    for (int i = 0; i < n; i++)
        maxx = max(maxx, arr[i]);
    return maxx;
}
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("max is %d.\n", find_max(arr, 5));
    return 0;
}