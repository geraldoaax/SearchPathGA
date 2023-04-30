#include <iostream>
#include "GeneticAlgorithm.h"
#include "SuperMatrix.h"
#include "SuperVector.h"
#include "Problem.h"
#include <time.h>
#include <omp.h>
#define POP_SIZE 30
#define DEFAULT_ORIGEM 2
#define MAX_GERATIONS 60
#define MAX_THREAD_POSSIBLE 16 // limit of thread
#define MIGRATE_AFTER_N_GERATIONS 8
#define MIGRATE_SIZE  5

//#define DebugPrint(x) cout<<x<<endl
#define DebugPrint(x)

#ifdef __linux__
#define MINE_TRUCKS_PATH "./MINE_PATHS.txt"
#define MINE_PATH "./"
#else// windows
#include <Windows.h>
#define MINE_TRUCKS_PATH "d:\\MINE_PATHS.txt"
#define MINE_PATH "d:\\"

#endif // __linux__

#define THIS_TRUCK 0



GeneticAlgorithm* truck[MAX_THREAD_POSSIBLE];
Problem* minaDinamica[MAX_THREAD_POSSIBLE];

using namespace std;

void Migrate();
void AddLog(uint16_t nCMS, float cTime);
uint8_t NumberOfThreads = 0;
uint16_t subPopSize = 0;

int main(int argc,  char** argv)
{
    try {
        const clock_t begin_time = clock();;
        uint16_t gerations = 0, i;
        uint8_t origem = DEFAULT_ORIGEM;
        individuo_t result;
        uint16_t nCMs = 0;

        NumberOfThreads =  2;
        subPopSize = POP_SIZE / NumberOfThreads;
        uint8_t MaxGerationsEach = MAX_GERATIONS;//(MAX_GERATIONS / NumberOfThreads) + 1;
        DebugPrint("Usando " << (int)NumberOfThreads << " threads e " << (int) MaxGerationsEach << " geracoes cada");
        DebugPrint("Populacao de " << (int)subPopSize << "individuos");


        if (argc < 4) //0 é o caminho do exe
        {
            cout << "Numero de parametros insuficiente, passe como parametro:" << endl;
            cout << "  \"Indice do caminhao\" \"indice da posicao atual\"" << endl;
            cout << "n de paramentrs " << (int)argc << endl;
            return (6);
        }
        else
        {
            origem = atoi(argv[2]);
            //cout<<" Pos ID = "<<atoi(argv[2])<<endl;
            nCMs = atoi(argv[3]);
            //cout<<" n° de CMs = "<<nCMS<<endl;
        }


        for (i = 0; i < NumberOfThreads; i++) {
            truck[i] = new GeneticAlgorithm(MINE_TRUCKS_PATH);
            minaDinamica[i] = new Problem();
            minaDinamica[i]->curTruck = atoi(argv[1]);
            //cout<<" Truck ID = "<<atoi(argv[1])<<endl;
            minaDinamica[i]->ncm = nCMs;
            //DebugPrint((int)origem << " " << (int)minaDinamica[i]->curTruck);
            //create a new population
            minaDinamica[i]->begin(MINE_PATH);
            truck[i]->Problema = minaDinamica[i];
            DebugPrint("Criando populacao " << (int)i);
            truck[i]->CreatePopulation(subPopSize, origem);
            DebugPrint("Criado");
        }


        gerations = 0;
        while (gerations < MaxGerationsEach)
        {

#pragma OMP_DYNAMIC parallel for private(i)
            for (i = 0; i < NumberOfThreads; i++)
            {
                //evolve the populations
                truck[i]->Evolve(MIGRATE_AFTER_N_GERATIONS);
            }
            Migrate();
            gerations += MIGRATE_AFTER_N_GERATIONS;
        }

        AddLog(nCMs, float( clock () - begin_time ) * 1000.0 /  CLOCKS_PER_SEC);
        std::cout  << (float( clock () - begin_time ) * 1000.0 /  CLOCKS_PER_SEC) << " ms";
        DebugPrint("\nFim da Evolucao\n");

        // result = truck[i]->theBest();
        // minaDinamica[i]->mainFunction(result, false);
        //result->printBackward();
        //std::cout<<endl <<"{"<< float( clock () - begin_time )*1000.0 /  CLOCKS_PER_SEC<<"ms}";


    } catch (const std::exception& e) { // reference to the base of a polymorphic object
        std::cout << "ERROR: " << e.what();

    }

    return 0;
}


void AddLog(uint16_t nCMS, float cTime)
{
    FILE * pFile;
#ifdef __linux__
    pFile = fopen ("./log.csv", "a"); // use "a" for append, "w" to overwrite, previous content will be deleted
#else
    pFile = fopen (".\log.txt", "a"); // use "a" for append, "w" to overwrite, previous content will be deleted
#endif
    fprintf (pFile, "%d;%f\n", nCMS, cTime); // character array to file
    fclose (pFile); // must close after opening
}



void Migrate()
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t NextIdx = 0;

    // Ordenate all sub populations
    for (i = 0; i < NumberOfThreads; i++)
    {
        truck[i]->SortByFitness();
    }

    for (i = 0; i < NumberOfThreads; i++)
    {
        NextIdx = (i + 1) % NumberOfThreads;
        for (j = 0; j < MIGRATE_SIZE; j++)
        {
            truck[NextIdx]->population[subPopSize - j - 1]->copy( truck[i]->population[j]);
        }
    }
}
