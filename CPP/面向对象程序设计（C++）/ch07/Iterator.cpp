#include <bits/stdc++.h>
using namespace std;
// 对考试结果进行统计的分析
class Iterator
{
  public:
    virtual ~Iterator() {}
    virtual bool operator!=(const Iterator &other) const = 0;
    virtual const Iterator &operator++() = 0;
    virtual const Iterator &operator++(int) = 0;
    virtual float &operator*() const = 0;
    virtual float *operator->() const = 0;
    bool operator==(const Iterator &other) const
    {
        return !(*this != other);
    }
};
class Collection
{
  public:
    virtual ~Collection() {}
    virtual Iterator *begin() const = 0;
    virtual Iterator *end() const = 0;
    virtual int size() = 0;
};
class ArryCollection : public Collection
{
    friend class arryIterator;
    float *_data;
    int _size;

  public:
    ArryCollection() : _size(10)
    {
        _data = new float[_size];
    }
    ArryCollection(int size, float *data) : _size(size)
    {
        _data = new float[_size];
        for (int i = 0; i < size; i++)
        {
            *(_data + i) = *(data + i);
        }
    }
    ~ArryCollection() { delete[] _data; }
    int size() { return _size; }
    Iterator *begin() const;
    Iterator *end() const;
};
Iterator *ArryCollection::begin() const
{
    return new ArryCollection(_data, 0);
}
Iterator *ArryCollection::end() const
{
    return new ArryCollection(_data, _size);
}
void analyze(Iterator *begin, Iterator *end)
{
    int passed = 0, count = 0;
    for (Iterator *p = begin; *p != *end; (*p)++)
    {
        if (**p >= 60)
            passed++;
        count++;
    }
    cout << "passing rate = " << (float)passed / count << endl;
}

int main()
{
    Collection *collection;
    analyze(collection->begin(), collection->end());
    return 0;
}