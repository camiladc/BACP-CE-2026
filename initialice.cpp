#include <bits/stdc++.h>

#include "global.h"
using namespace std;


individual initialize_ind(){

    individual new_ind;
    new_ind.courses = {};
    new_ind.fitness[0] = 0.0;
  
    return new_ind;
}

vector<individual> initialize_pop(){
    
    vector<individual> pop = {};
    for(int i = 0; i < params.popsize; i++){
        individual new_ind = initialize_ind();
        pop.push_back(new_ind);

    }

    return pop;
}