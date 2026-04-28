#include<bits/stdc++.h>

#include "global.h"
using namespace std;

// Function to sort individuals in ascending order of fitness
bool fitnessComparisonAsc(const individual &a, individual &b) {
    return a.fitness[0] < b.fitness[0];
}

individual selection(vector<individual> &old_pop){

}

// Function to generate a new population from the old population using selection, crossover, and mutation
void generateNewPop(vector<individual> &old_pop){

    vector<int> a1(params.popsize+1), a2(params.popsize+1);
    int temp;
    int rand;
    individual parent1, parent2;
    vector<individual> new_pop(params.popsize), children;

    double p;
    sort(old_pop.begin(), old_pop.end(), fitnessComparisonAsc);

    //Copy elite individuals to the new population
    for (int i = 0; i < params.elite; i++){
        new_pop[i] = old_pop[i];
    }

    for (int c = params.elite; c < params.popsize; c++){
        parent1 = selection(old_pop);

        p = getRandomProb();
        if(p<=params.cross_prob){
            parent2 = selection(old_pop);

            children = crossover(parent1, parent2);

            //If the population size is odd, we only add one of the children to the new population
            if(c == params.popsize-1){
                float lucky = getRandomProb();
                if(lucky <= 0.5){
                    new_pop[c] = children[0];
                }
                else{
                    new_pop[c] = children[1];
                }
            }
            else{
                new_pop[c] = children[0]; new_pop[c+1] = children[1];
            }
            c++;
        }
        else if(p<=(params.cross_prob + params.mut_prob)){

            mutation(parent1);

            new_pop[c] = parent1;
        }
        else
            new_pop[c] = parent1;
    }
    old_pop = new_pop;
}

// Function to evaluate the fitness of each individual in the population
void evaluatePop(vector<individual> &pop){
}
