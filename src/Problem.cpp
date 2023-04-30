#include "Problem.h"
#include <iostream>
#include <fstream>
#include <math.h>

#define     DMT_FILENAME        "MINE_DMTS.txt"
#define     FRENTES_FILENAME    "MINE_FRENTES.txt"
#define     INT_FILENAME        "MINE_INT.txt"
#define     DESTINOS_FILENAME   "MINE_DESTINOS.txt"
#define     CMS_FILENAME        "MINE_CMS.txt"

using namespace std;

Problem::Problem()
{
    //ctor
}

Problem::~Problem()
{
    //dtor
}

void Problem::AddCM(infoList_t* Info, bool canClear)
{
    uint16_t line;

    if (canClear)
        Info->clear();
    line = Info->size();
    Info->push_back(new info_t);

    Info->at(line)->push_back(line);
    Info->at(line)->push_back(34);
    Info->at(line)->push_back(35);
    Info->at(line)->push_back(11);
    Info->at(line)->push_back(1);
}

void Problem::begin(std::string  path)
{
	int i=0;
	
    #ifdef __linux__
        DebugPrint("Executando no linux\n");
    #else
        //DebugPrint("Executando no Windows\n");
    #endif // __linux__

    readCommaText(path+DMT_FILENAME,&dmt);
    //cout<<path+DMT_FILENAME<<endl;
    dmt.pop_back();
    //cout<< "DMT size "<<(int)dmt.size()<<endl;

    readCommaText(path+FRENTES_FILENAME,&frentes);
    frentes.pop_back();

    readCommaText(path+FRENTES_FILENAME,&Full);
    Full.pop_back();

    readCommaText(path+INT_FILENAME,&Full,false);
    Full.pop_back();

    readCommaText(path+DESTINOS_FILENAME,&Full,false);
    Full.pop_back();


    DebugPrint("Full size "<<(int)Full.size());

	cms.clear();
    for (i = 0; i < ncm; i++)
    {
        AddCM(&cms, false);
    }
    //readCommaText(path+CMS_FILENAME,&cms);
    
    DebugPrint("CM size "<<(int)cms.size());

    DebugPrint("Processing informations");
    ProcessInformations();
    DebugPrint("ProcessInformations finished");


}

double Problem::GetDmt(uint16_t origem,uint16_t destino)
{
    double value=0.0;
    value = dmt.at(origem)->at(destino);
    return value;
}

double Problem::mainFunction(SuperVector* value, bool verbose)
{
    double fitness;
    uint8_t cfrente = value->back();

    double d = Distance(value);
    if(d == 0)
    {
        DebugPrint("INDIVIDUO DE DISTANCIA NULA");
    }


    double tc = Full.at(cfrente)->at(3)/3600.0;
    double tTransport = d/cms.at(curTruck)->at(1) + tc;
    double tDelay = CalcQueueTime(tTransport,cfrente)/60.0;
    double cExtraction = fabs(Full.at(cfrente)->at(4) - Full.at(cfrente)->at(1))/Full.at(cfrente)->at(1);
    if (cExtraction > 1)
    {
        cExtraction = 0;
    }

    fitness = exp(-tTransport*10.0)+ cExtraction+exp(-tDelay);
    //fitness = cExtraction;
    if(fitness == 0)
    {
        return fitness;
    }

    if(verbose)
    {
        double Pd  = 100.0*exp(-d)/fitness ;
        double Ptt = 100.0*exp(-tTransport*10.0)/fitness;
        double Pex = 100.0*cExtraction/fitness;
        double Pdy = 100.0*exp(-tDelay)/fitness;
        cout<<"<"<<Ptt<<"%; "<<Pex<<"%; "<<Pdy<<"%; "<<fitness<<">";


    }

    //fitness =  exp(-d);//exp(-cExtraction/1000.0);
    //cout<<" :"<<cExtraction<<": ";
    return fitness;
}




void Problem::ProcessInformations()
{
    uint16_t i=0;
    double value;
    uint8_t cfrente;

    for(i=0;i<cms.size();i++)
    {
        DebugPrint((int)i);

        cfrente = cms.at(i)->at(3);
        DebugPrint("Frente size "<<(int)Full.size());
        DebugPrint("cm size "<<(int)cms.size());
        DebugPrint("cfrente "<<(int)cfrente);

        // Tempo de carregamento
        //value = dmt(i)/vm(i) +Tc (minutos)
        value = cms.at(i)->at(4)/(60.0*cms.at(i)->at(1));//+ Full.at(cfrente)->at(3);
        DebugPrint("value");
        cms.at(i)->push_back(value);
        DebugPrint("cms");
    }
}


void Problem::readCommaText(std::string filename,infoList_t* Info, bool canClear)
{
    int16_t line=-1;
    double buf=0.0;
    //cout<<filename<<endl;
    if (canClear)
        Info->clear();
    std::string str;
    std::stringstream ss(str);

    ifstream infile;
    std::ifstream file( filename.c_str() );

    if(!file)
    {
        DebugPrint("ERROR! OPEN FILE");
        return;
    }

    ss << file.rdbuf();
    infile.close();

    line++;
    Info->push_back(new info_t);
    while (ss >> buf)
    {
        Info->at(line)->push_back(buf);

        if (ss.peek() == LIMITER)
            ss.ignore();
        if (ss.peek() == NEW_LINE)
        {
            ss.ignore();
            line++;
            Info->push_back(new info_t);
        }
    }
}


double Problem::Distance(SuperVector* ind)
{
    uint16_t i;
    double value=0.0;

    if(ind->size() < 2)
    {
        DebugPrint("Distancia nula por tamanho do ind = "<<(int) ind->size());
        return 0;
    }

    for(i=0;i<ind->size()-1;i++)
    {
        //ind->print();
        value+= GetDmt(ind->at(i),ind->at(i+1));
        //DebugPrint("d["<<(int)i<<"] = "<<value);
    }


    return value;
}

double Problem::CalcQueueTime(double T,uint8_t cfrente)
{
    uint16_t i=0;
    bool repeat=false;
    double tc = frentes.at(cfrente)->at(3);
    double value;
    double delay = tc;

    SuperVector avaiable;
    for(i=0;i<cms.size();i++)
    {
        if(cms.at(i)->at(0)==curTruck)
        {
            continue;
        }
        if(cms.at(i)->at(3)==cfrente)
        {
            value = cms.at(i)->at(5);
            if(T >= value)
            {
                avaiable.add(value);
            }
        }
    }

    do
    {
        repeat=false;
        for(i=0;i<avaiable.size();i++)
        {
            if(T - avaiable.at(i) <= delay)
            {
                delay +=tc;
                avaiable.del(i);
                repeat = true;
                break;
            }
        }
    }while (repeat);
    return delay;
}
