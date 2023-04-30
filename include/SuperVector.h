#ifndef SUPERVECTOR_H
#define SUPERVECTOR_H

using namespace std;

#include <stdint.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define VEC_TYPE uint8_t

typedef std::vector<VEC_TYPE> IntContainer;
typedef IntContainer::iterator IntIterator;


class SuperVector: public std::vector<VEC_TYPE>
{
    public:
        SuperVector();
        virtual ~SuperVector();
        int indexOf(const VEC_TYPE elem);
        bool Contains(const VEC_TYPE elem);
        void print();
        void printBackward();
        void add(const VEC_TYPE elem);
        void del(const size_t index);
        void del(const size_t index,const size_t count);
        void delBack();
        bool ignoreDuplicates;
        void swp(const size_t index1,const size_t index2);
        void remove(const VEC_TYPE elem);
        void removeAll(const VEC_TYPE elem);
        void delLeft(const size_t startIndex);
        void delRight(const size_t startIndex);
        void copy(SuperVector* v);
        void swpRange(SuperVector* v1,SuperVector* v2,size_t IndexBegin);
        int findFirstDup();
    protected:

    private:
};



#endif // SUPERVECTOR_H
