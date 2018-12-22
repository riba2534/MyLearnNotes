int find_min(int arr[], int n)
{
    int minn = arr[0];
    for (int i = 0; i < n; i++)
        minn = minn < arr[i] ? minn : arr[i];
    return minn;
}