int find_max(int arr[], int n)
{
    int maxx = 0;
    for (int i = 0; i < n; i++)
        maxx = maxx > arr[i] ? maxx : arr[i];
    return maxx;
}