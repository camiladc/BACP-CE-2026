#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <fstream>

/* ------------- STRUCTURES ---------------------*/
struct individual
{
    // Individual representation
    std::vector<int> courses;
    
    // Fitness value
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

// Random functions
void randomize(int seed);
int getRandomInt(int a, int b);
float getRandomProb();
float getRandomProb_lim(float lim);

// Reader functions
void readInstance(std::ifstream &f);

// Evolutive Algorithm functions
individual initialize_ind();
std::vector<individual> initialize_pop();
bool fitnessComparisonAsc(const individual &a, individual &b);
void generateNewPop(std::vector<individual> &old_pop);
void evaluatePop(std::vector<individual> &pop);
individual selection(vector<individual> &old_pop);
std::vector<individual> crossover(individual &parent1, individual &parent2);
void mutation(individual &parent1);

// Writer functions
void writeInd(individual &ind);


/* ------------- GLOBAL VARIABLES ---------------------*/

// Hyperparams declaration
extern hyperparams params;

// Random number generator declaration
extern std::mt19937 rng;

// Instance data
extern int num_periods; //number of periods in the curriculum
extern int min_load; // minimum academic load allowed per period
extern int max_load; // maximum academic load allowed per period
extern int min_courses; // minimum amount of courses allowed per period
extern int max_courses; // maximum amount of courses allowed per period
extern int num_courses; // number of courses in the curriculum
extern std::vector<std::string> course_names; // list of course names, indexed by course ID
extern std::vector<int> course_credits; // list of course credits, indexed by course ID
extern std::vector<std::pair<int,int>> prerequisites; // list of prerequisites, indexed by course ID
extern std::unordered_map<std::string,int> course_index; // map of course names to IDs
extern std::vector<std::vector<int>> prereq_adj; // adjacency list of prerequisites


#endif