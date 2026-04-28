#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <fstream>

/* ------------- STRUCTURES ---------------------*/
struct individual
{
    // individual representation
    std::vector<int> courses;
    
    // fitness value
    float fitness[1];

};

struct hyperparams{
    int popsize;
    int max_gen;
    int elite;
    float cross_prob;
    float mut_prob;
};

/* ------------- FUNCTION PROTOTYPES ---------------------*/

/* Random functions */
void randomize(int seed);
int getRandomInt(int a, int b);
float getRandomProb();
float getRandomProb_lim(float lim);

/* Reader functions */
void readInstance(std::ifstream &f);

/* Evolutive Algorithm functions */
individual initialize_ind();
std::vector<individual> initialize_pop();
bool fitnessComparisonAsc(const individual &a, individual &b);
void generateNewPop(std::vector<individual> &old_pop);
void evaluatePop(std::vector<individual> &pop);
individual selection(vector<individual> &old_pop);
std::vector<individual> crossover(individual &parent1, individual &parent2);
void mutation(individual &parent1);

/* Writer functions */
void writeInd(individual &ind);


/* ------------- GLOBAL VARIABLES ---------------------*/

/* Hyperparams declaration */
extern hyperparams params;

/* Random number generator declaration */
extern std::mt19937 rng;


#endif