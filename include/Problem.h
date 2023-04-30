#ifndef PROBLEM_H
#define PROBLEM_H

#include "SuperVector.h"
#include <string>
#include <sstream>


#ifdef __linux__
  #define NEW_LINE    (char)13
#else
  #define NEW_LINE    (char)10
#endif

#define LIMITER     ','

typedef std::vector<double> info_t;
typedef std::vector<info_t*> infoList_t;

//#define DebugPrint(x) cout<<x<<endl
#define DebugPrint(x)

class Problem
{
    public:
        Problem();
        virtual ~Problem();
        virtual double mainFunction(SuperVector* value, bool verbose=false);
        virtual void begin(std::string path);
		void AddCM(infoList_t* Info, bool canClear);
        infoList_t frentes;
        infoList_t dmt;
        infoList_t cms;
        uint8_t curTruck;
        infoList_t Full;
		int ncm;
    protected:

    private:
        double GetDmt(uint16_t origem,uint16_t destino);
        double CalcQueueTime(double T,uint8_t cfrente);
        double Distance(SuperVector* ind);
        void ProcessInformations();
        virtual void readCommaText(std::string filename,infoList_t* Info, bool canClear=true);
};

#endif // PROBLEM_H
