#ifdef __linux__
#include "GeneticAlgorithm.h"
#else
#include <GeneticAlgorithm.h>
#endif
//#define DebugPrint(x) cout<<x<<endl
#define DebugPrint(x)

GeneticAlgorithm::GeneticAlgorithm(char* mat_filename)
{
    srand (time(NULL));
    pbMutation  = 0.1;
    pbCrossover = 0.6;
    gerations   = 0;
    pop_size    = 0;
    torneioFactor = 0.2;
    elitismRate   = 0.1;
    sorted = false;
    maxPopulation = 30;
    _mat_filename = mat_filename;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    //dtor
}


void GeneticAlgorithm::CreatePopulation(const size_t sz, const uint8_t origem)
{
    SuperVector* ind;
    size_t i;
    maxPopulation = sz;

    //define destinies
    for (i = 0; i < Problema->frentes.size(); i++)
    {
        destinos.add((uint8_t)Problema->frentes.at(i)->at(0));
    }

    DebugPrint("Clear population");
    population.clear();


    for (i = 0; i < sz; i++)
    {
        ind = new SuperVector();
        ind->add(origem);
        CreateIndividual(ind, destinos);

        population.push_back(ind);
        //DebugPrint("Ind "<<(int)i);
        //ind->print();
        //DebugPrint("");
    }

    gerations = 0;
    pop_size = population.size();
    //DebugPrint("pop size "<<population.size());

    CalcFitness();
    sorted = false;
}

size_t GeneticAlgorithm::Select()
{
    return Tournament();
}

void GeneticAlgorithm::GetCommunIndexs(individuo_t p1, individuo_t p2, individuo_t communs)
{
    size_t i, j;

    communs->clear();
    for (i = 0; i < p1->size(); i++)
    {
        for (j = 0; j < p2->size(); i++)
        {
            if (p1[i] == p2[j])
            {
                communs->add(p1->at(i));
                break;
            }
        }
    }
}

void GeneticAlgorithm::Swap(individuo_t p1, individuo_t p2, individuo_t f1, individuo_t f2, uint8_t elem)
{
    size_t i;
    size_t p1Idx = p1->indexOf(elem);
    size_t p2Idx = p2->indexOf(elem);

    for (i = 0; i < p1Idx; i++)
    {
        f1->add(p1->at(i));
    }

    for (i = 0; i < p2Idx; i++)
    {
        f2->add(p2->at(i));
    }

    for (i = p1Idx; i < p2->size(); i++)
    {
        f1->add(p2->at(i));
    }

    for (i = p2Idx; i < p1->size(); i++)
    {
        f2->add(p1->at(i));
    }
}

void GeneticAlgorithm::fixIndividual(individuo_t fn)
{
    SuperVector dest;
    int idx =  fn->findFirstDup();
    if (idx != -1)
    {
        dest.add(fn->back());
        fn->del(idx, fn->size());
        CreateIndividual(fn, dest);
    }
}


bool GeneticAlgorithm::CrossoverIndividual(individuo_t p1, individuo_t p2, individuo_t f1, individuo_t f2 )
{
    bool canCrossover;
    size_t crossIndex;
    SuperVector communIndexs;


    canCrossover = rand() < pbCrossover;

    if (canCrossover)
    {
        GetCommunIndexs(p1, p2, &communIndexs);

        if (communIndexs.size() == 0)
        {
            return false;
        }

        crossIndex = RandomInt(communIndexs.size());

        Swap(p1, p2, f1, f2, communIndexs[crossIndex]);

        fixIndividual(f1);
        fixIndividual(f2);
    }
    else
    {
        f1->copy(p1);
        f2->copy(p2);

    }

    return true;
}

void GeneticAlgorithm::Crossover()
{
    individuo_t p1, p2, f1, f2;
    size_t elite = (size_t)(elitismRate * maxPopulation);

    Elitism(elite);


    while (newPopulation.size() < maxPopulation)
    {
        p1 = population[Select()];
        p2 = population[Select()];
        f1 = new SuperVector;
        f2 = new SuperVector;

        CrossoverIndividual(p1, p2, f1, f2);


        newPopulation.push_back(f1);
        newPopulation.push_back(f2);
    }

    sorted = false;
}

void GeneticAlgorithm::MutateIndividuo(individuo_t ind)
{
    size_t sz = ind->size();
    size_t index = RandomInt(sz) - 1;
    ind->del(index, sz);
    CreateIndividual(ind, destinos);
}

void GeneticAlgorithm::Mutate()
{
    size_t i, elite;
    bool canMutate;
    elite = (size_t)(elitismRate * population.size());

    for (i = elite; i < population.size(); i++)
    {
        canMutate = rand() < pbMutation;

        if (canMutate)
        {
            MutateIndividuo(population[i]);
        }
    }
}


void  GeneticAlgorithm::ReadMatFromFile(char* filename)
{
    ReadCommaText(filename, &Mat);
}



void GeneticAlgorithm::ReadCommaText(char* filename, SuperMatrix* Info)
{
    int16_t line = -1;
    int buf = 0;

    Info->clear();
    std::string str;
    std::stringstream ss(str);

    ifstream infile;
    std::ifstream file( filename );

    if (!file)
    {
        DebugPrint("ERROR! OPEN FILE");
        return;
    }

    ss << file.rdbuf();
    infile.close();

    line++;
    Info->push_back(new SuperVector);
    while (ss >> buf)
    {
        if (buf != -1)
            Info->at(line)->push_back((int)buf);

        if (ss.peek() == LIMITER)
            ss.ignore();
        if (ss.peek() == NEW_LINE)
        {
            ss.ignore();
            line++;
            Info->push_back(new SuperVector);
        }
    }
    DebugPrint("Mat Size = " << (int)Info->size());
}



void GeneticAlgorithm::CreateIndividual(SuperVector* ind, SuperVector destinos)
{

    uint8_t origem, id = 0;
    uint8_t index, i;

    ReadMatFromFile(_mat_filename);
    Mat.removeAll(ind);
    origem = ind->back();

    DebugPrint("Starting create ind");
    //Mat.print();
    do
    {
        if (Mat[origem]->size() > 0)
        {
            index = RandomInt(Mat[origem]->size());
            DebugPrint("index " << (int)index);
            id = Mat[origem]->at(index);
            DebugPrint("id " << (int)id);
            ind->add(id);
            Mat.removeAll(id);
            origem = id;
        }
        else
        {
            while (Mat[origem]->size() == 0)
            {
                ind->delBack();
                origem = ind->back();
                if (ind->size() == 0)
                {
                    DebugPrint("no paths");
                    return ;
                }
            }
        }

    } while (!destinos.Contains(id));
    DebugPrint("finished create ind");
}

int16_t GeneticAlgorithm::RandomInt(uint16_t max)
{

    uint16_t value;
    value = rand() % max;
    return value;
}

void GeneticAlgorithm::Evolve(uint8_t n)
{
    uint8_t i = 0;
    for (i = 0; i < n; i++)
    {
        Evolve();
    }
};

void GeneticAlgorithm::Evolve()
{
    Crossover();
    Mutate();
    CalcFitness();
}

double GeneticAlgorithm::fitnessFunction(individuo_t ind)
{
    return Problema->mainFunction(ind);
}

void GeneticAlgorithm::Exchange(size_t index1, size_t index2)
{
    swap(fitness[index1], fitness[index2]);
    population.swp(index1, index2);
}


void GeneticAlgorithm::SortByFitness()
{
    size_t i, j;
    for (i = 0; i < fitness.size() - 1; i++)
    {
        for (j = i + 1; j < fitness.size(); j++)
        {
            if (fitness[i] < fitness[j])
            {
                Exchange(i, j);
            }
        }
    }
    sorted = true;
}

void GeneticAlgorithm::CalcFitness()
{
    size_t i;
    totalFitness = 0.0;

    //DebugPrint("pop size"<<(int)population.size());

    for (i = 0; i < pop_size; i++)
    {
        if (fitness.size() < i + 1)
        {
            //DebugPrint("Tring pushback fitness linha= "<<(int)i);
            fitness.push_back(fitnessFunction(population[i]));
            //DebugPrint("Pushback fitness ok");
        }
        else
        {
            fitness[i] = fitnessFunction(population[i]);
        }
        totalFitness += fitness[i];
    }
    //DebugPrint("fitness size after"<<(int)fitness.size());
}


void GeneticAlgorithm::CalcProbabillity()
{
    double _probability = 0;
    size_t i;

    cumulativeProbability.clear();
    for ( i = 0; i < pop_size; i++)
    {
        _probability = fitness[i] / totalFitness;
        if (i == 0)
        {
            cumulativeProbability.add(_probability);
        }
        else
        {
            cumulativeProbability.add(cumulativeProbability[i - 1] + _probability);
        }
    }
}

size_t GeneticAlgorithm::Tournament()
{
    size_t sz, i;
    size_t result;
    size_t index;
    sz = pop_size * torneioFactor;
    if (sz == 0) return (0);
    result = RandomInt(pop_size);
    if (sz > 0)
    {
        for (i = 1; i < sz; i++)
        {
            index = RandomInt(pop_size);
            if ( fitness[index] > fitness[result])
            {
                result = index;
            }
        }
    }
    return result;
}


void GeneticAlgorithm::Elitism(size_t size)
{
    size_t i, j;

    if (!sorted)
    {
        SortByFitness();
    }
    newPopulation.clear();



    for ( i = 0; i < size; i++)
    {
        newPopulation.push_back(new SuperVector);
        newPopulation.back()->copy(population[i]);
    }
}

individuo_t GeneticAlgorithm::theBest()
{
    SortByFitness();
    return (population[0]);
}

