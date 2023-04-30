#include "SuperVector.h"

SuperVector::SuperVector()
{
    //ctor
    ignoreDuplicates =false;
}

SuperVector::~SuperVector()
{
    //dtor
}

int SuperVector::indexOf(const VEC_TYPE elem)
{
    IntIterator i = find(begin(), end(), elem);
    if(i != end())
    {
        return (i-begin());
    }
    else
    {
        return -1;
    }
}

bool SuperVector::Contains(const VEC_TYPE elem)
{
    return (indexOf(elem) > -1);
}

void SuperVector::print()
{
    size_t i;
    if(size()>200)
    {
        cout<<"overlow"<<size() <<endl;
        return;
    }
    cout << "[";
    for(size_t i=0;i<size();i++)
    {
        if(i>0)
        {
            cout << ",";
        }
        cout<< (int)at(i);
    }
    cout<<"]";
}

void SuperVector::printBackward()
{
    int i;
    if(size()>200)
    {
        cout<<"overlow"<<size() <<endl;
        return;
    }
    cout << "[";
    for(i=size()-1;i>=0;i--)
    {
        if(i<size()-1)
        {
            cout << ",";
        }
        cout<< (int)at(i);
    }
    cout<<"]";
}


void SuperVector::add(const VEC_TYPE elem)
{
    if((!ignoreDuplicates)||(!Contains(elem)))
    {
        push_back(elem);
    }
}

void SuperVector::del(const size_t index)
{
  if(index < size())
  {
      erase(begin()+index);
  }
}

void SuperVector::del(const size_t index,const size_t count)
{
  IntIterator ed,bg;
  if(index < size())
  {
    bg = begin()+index;
    if((index+count)> size())
    {
        ed = end();
    }
    else
    {
        ed = bg+count;
    }
    erase(bg,ed);
  }
}

void SuperVector::swp(const size_t index1,const size_t index2)
{
    if(index1 != index2)
    {
        iter_swap(begin() + index1, begin() + index2);
    }
}

void SuperVector::remove(const VEC_TYPE elem)
{
    int idx= indexOf(elem);
    if (idx != -1)
    {
        del(idx);
    }
}

void SuperVector::removeAll(const VEC_TYPE elem)
{
    int idx;
    do
    {
        idx= indexOf(elem);
        if (idx != -1)
        {
            del(idx);
        }
    }while(idx != -1);
}

void SuperVector::delBack()
{
    erase(end()-1);
}

void SuperVector::delLeft(const size_t startIndex)
{
    erase(begin(), begin()+startIndex);
}

void SuperVector::delRight(const size_t startIndex)
{
    erase(begin()+startIndex,end());
}

void SuperVector::copy(SuperVector* v)
{
    assign(v->begin(),v->end());
}

void SuperVector::swpRange(SuperVector* v1,SuperVector* v2,size_t IndexBegin)
{
    size_t i;
    v1->swap(*v2);

    for(i=0;i<IndexBegin;i++)
    {
        //std:swap(v1->at(i),v2->at(i));
    }
}

int SuperVector::findFirstDup()
{
    size_t i,j;
    for(i=1;i<size();i++)
    {
        for(j=0;j<i;j++)
        {
            if(at(j)==at(i))
            {
                return (i);
            }

        }
    }
    return (-1);
}
