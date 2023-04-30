#ifndef SUPERMATRIX_H
#define SUPERMATRIX_H

#include <stdint.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "SuperVector.h"


class SuperMatrix:public std::vector<SuperVector*>
{
    public:
        SuperMatrix();
        virtual ~SuperMatrix();
        void removeAll(const VEC_TYPE elem);
        void removeAll(SuperVector* vec);
        void swp(const size_t index1,const size_t index2);
        void copy(const size_t index1,const size_t index2);
        void copy(const size_t index, SuperVector* v);
        void freeAndClear();
        void print();
    protected:

    private:
};

#endif // SUPERMATRIX_H
