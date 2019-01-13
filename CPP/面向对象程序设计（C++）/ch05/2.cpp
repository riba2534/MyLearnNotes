#include <bits/stdc++.h>
using namespace std;
class Matrix
{
    int _size;  //方阵规模
    int *_data; //数据空间
    int row, col;
    char dir;

  public:
    Matrix(int size); //构造函数
    ~Matrix();        //析构
    void fill();
    virtual int findPosition() = 0;
    friend ostream &operator<<(ostream &out, const Matrix &m); //重载运算符
};
Matrix::Matrix(int size) : _size(size), row(-1), col(0), dir('D')
{
    _data = new int[size * size];
    memset(_data, 0, sizeof(int) * _size * _size);
}
Matrix::~Matrix()
{
    delete[] _data;
}
ostream &operator<<(ostream &out, const Matrix &m)
{
    for (int r = 0; r < m._size; r++)
    {

        for (int c = 0; c < m._size; c++)
            cout << *(m._data + r * m._size + c) << '\t';
        cout << endl;
    }
}
void Matrix::fill()
{
    for (int num = 1; num <= _size * _size; num++)
    {
        int pos = findPosition();
        _data[pos] = num;
    }
}
int Matrix::findPosition()
{
    switch (dir)
    {
    case 'D':
        if (row < _size - 1 && _data[(row + 1) * _size + col] == 0)
            row++;
        else
        {
            dir = 'R';
            col++;
        }
        break;
    case 'R':
        if (col < _size - 1 && _data[row * _size + col + 1] == 0)
            col++;
        else
        {
            dir = 'U';
            row--;
        }
        break;
    case 'U':
        if (row > 0 && _data[(row - 1) * _size + col] == 0)
            row--;
        else
        {
            dir = 'L';
            col--;
        }
        break;
    case 'L':
        if (col > 0 && _data[row * _size + col - 1] == 0)
            col--;
        else
        {
            dir = 'D';
            row++;
        }
    }
    return row * _size + col;
}

class ClockwiseMatrix : public Matrix
{
    int row, col;
    char dir;

  public:
    int findPosition();
};
class RowFirstMatrix : public Matrix
{
    int next;

  public:
    int findPosition()
    {
        return next++;
    }
};
int main()
{
    Matrix s(8);
    s.fill();
    cout << s << endl;
    return 0;
}