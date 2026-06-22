#include<bits/stdc++.h>
#include <fstream>
#include <chrono>

#include "global.h"
using namespace std;

// Hyperparams declaration
hyperparams params;

// Random number generator declaration
mt19937 rng;

int main(int argc, char *argv[]){

    if(argc < 6){
        cout << "Usage:\n./main base_instance_path case_instance_path seed max_gen popsize cross_prob mut_prob \n";
        exit(1);
    }

    // Assign the path
    string base_path = argv[1];
    string case_path = argv[2];
  
    // Assign the seed
    int seed = atoi(argv[3]);
    randomize(seed);

    // Define the hyperparameters
    params.max_gen = atoi(argv[4]);
    params.popsize = atoi(argv[5]);
    params.cross_prob = stof(argv[6]);
    params.mut_prob = stof(argv[7]);
    params.elite = 1;
    
    // Reading the instance file
    ifstream file_base(base_path);
    ifstream file_case(case_path);

    if(!file_base.is_open()){
        cout << "\nCannot open file: " << base_path << endl;
        exit(1);
    }

    if (!file_case.is_open()) {
        cerr << "\nCannot open file: " << case_path << endl;
        exit(1);
    }

    cout << "Reading instance: " << case_path << "\n";
    readInstance(file_base, file_case);
    
    // Start the timer
    auto start = chrono::high_resolution_clock::now();

    // Initialize the population
    vector<individual> pop = initialize_pop();

    // Main loop of the evolutionary algorithm
    for (int i = 0; i < params.max_gen; i++)
    {      
        generateNewPop(pop);
        evaluatePop(pop);
    }

    // End the timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;


    // Mostrar la mejor solución encontrada
    sort(pop.begin(), pop.end(), fitnessComparisonAsc);
    cout << "\nBest solution found:" << endl;
    writeInd(pop[0]);
    
    cout<<endl<<"Time elapsed: "<<elapsed.count()<<" seconds"<<endl;

    file_base.close();
    file_case.close();

    return 0;

}