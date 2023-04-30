#ifndef TRUCKLIST_H
#define TRUCKLIST_H

#include <stdint.h>
#include <vector>


using namespace std;

typedef struct sTruckInfo
{
    int paramTest;
} TTruckInfo;

class TruckList: public std::vector<TTruckInfo*>
{
    public:
        TruckList();
        virtual ~TruckList();
        void loadFromFile(char* fileName);
    protected:

    private:
};

#endif // TRUCKLIST_H
