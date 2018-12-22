#include "foo.h"
#include "bar.h"
#include <stdio.h>
int main()
{
    int arr[5] = {1, 8, 6, 3, 4};
    printf("min is %d.\n", find_min(arr, 5));
    return 0;
}