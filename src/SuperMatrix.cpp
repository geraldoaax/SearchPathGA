#include "SuperMatrix.h"

SuperMatrix::SuperMatrix()
{
    //ctor
}

SuperMatrix::~SuperMatrix()
{
    //dtor
}

void SuperMatrix::removeAll(const VEC_TYPE elem)
{
    size_t i;
    for(i=0;i<size();i++)
    {
        at(i)->remove(elem);
    }
}

void SuperMatrix::removeAll(SuperVector* vec)
{
    size_t i;
    for(i=0;i< vec->size();i++)
    {
        removeAll(vec->at(i));
    }
}

void SuperMatrix::swp(const size_t index1,const size_t index2)
{
    at(index1)->swap(*at(index2));
}

void SuperMatrix::copy(const size_t indexTo,const size_t indexFrom)
{
    at(indexTo)->assign(at(indexFrom)->begin(),at(indexFrom)->end());
}


void SuperMatrix::copy(const size_t index, SuperVector* v)
{
    at(index) = (new SuperVector());
    at(index)->assign(v->begin(),v->end());
}

void SuperMatrix::freeAndClear()
{
    size_t i;
    for(i=0;i<size();i++)
    {
        free(at(i));
    }
    clear();
}


void SuperMatrix::print()
{
    size_t i;

    for(i=0;i< size();i++)
    {
        at(i)->print();
        cout<<endl;
    }
}
