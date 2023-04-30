#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <iostream>
#include <vector>
#include <algorithm>

#ifdef __linux__
	#include "Problem.h"
	#include "SuperVector.h"
	#include "SuperMatrix.h"
#else
	#include <Problem.h>
	#include <SuperVector.h>
	#include <SuperMatrix.h>
#endif


#include <fstream>
#include <time.h>



typedef SuperVector* individuo_t;

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm(char* mat_filename);
        virtual ~GeneticAlgorithm();
        //****************Main***Functions*****************
        SuperMatrix population;
        void CreatePopulation(const size_t sz, const uint8_t origem);
        void Evolve();
        void Evolve(uint8_t n);
        void CalcFitness();
        void SortByFitness();
        size_t Tournament();
        void CalcProbabillity();
        void Elitism(size_t size);
        void Mutate();
        individuo_t theBest();

        //*************************************************
        void ReadMatFromFile(char* filename);
        void CreateIndividual(SuperVector* ind,SuperVector destinos);
        SuperVector destinos;
        vector<double> fitness;
        double pbMutation;
        double pbCrossover;
        double torneioFactor;
        double elitismRate;
        uint32_t gerations;
        size_t maxPopulation;
        Problem* Problema;
    protected:

    private:
        void ReadCommaText(char* filename,SuperMatrix* Info);
        double fitnessFunction(individuo_t ind);
        void MutateIndividuo(individuo_t ind);
        void fixIndividual(individuo_t fn);
        void Swap(individuo_t p1, individuo_t p2,individuo_t f1, individuo_t f2,uint8_t elem);
        void GetCommunIndexs(individuo_t p1, individuo_t p2,individuo_t communs);
        int16_t RandomInt(uint16_t max);
        size_t Select();
        bool CrossoverIndividual(individuo_t p1, individuo_t p2,individuo_t f1, individuo_t f2 );
        void Crossover();
        void Exchange(size_t index1, size_t index2);
        SuperVector cumulativeProbability;
        SuperMatrix newPopulation;
        SuperMatrix Mat;
        size_t pop_size;
        double totalFitness;
        bool sorted;
        char* _mat_filename;
};

#endif // GENETICALGORITHM_H
