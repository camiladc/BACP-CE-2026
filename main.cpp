#include<bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <format>
#include <errno.h>
#include <filesystem>

#include "global.h"
using namespace std;
namespace fs = std::filesystem;

// Hyperparams declaration
hyperparams params;
// period multiplier
float period_multiplier = 1.5;

// Random number generator declaration
mt19937 rng;

int main(int argc, char *argv[]){

    if(argc < 9){
        cout << "Usage:\n./main base_instance_path seed max_gen popsize period_multiplier cross_prob mut_prob mut_branch_prob mut_ins_prob \n";
        exit(1);
    }

    // Assign the path
    // inputs
    string base_path = argv[1];

    //outputs
    string stem = fs::path(base_path).stem().string();
    string f_convergence = format("convergence_results/convergence_{}.csv", stem); // convergence graph
    string f_sol = format("solutions/solutions_{}.csv", stem); // formated solution as csv
    
    ofstream conv(f_convergence, ios::out);
    ofstream solution(f_sol, ios::out);

    if(conv.is_open())
    {
        conv.flush();
    }
    else
    {
        cerr << "Failed to create file" << f_convergence << ": " << errno << endl;
    }

    if(solution.is_open())
    {
        solution.flush();
    }
    else
    {
        cerr << "Failed to create file" << f_sol << ": " << errno << endl;
    }
    
    conv << "# gen;best_fit\n";
    solution << "id_sol;period;courses;total_credits;period_fit;total_fit\n";

    // Assign the seed
    int seed = atoi(argv[2]);
    randomize(seed);
    
    // Define the hyperparameters
    params.max_gen = atoi(argv[3]);
    params.popsize = atoi(argv[4]);
    period_multiplier = stof(argv[5]);
    params.cross_prob = stof(argv[6]);
    params.mut_prob = stof(argv[7]);
    params.mut_branch_prob = stof(argv[8]);
    params.mut_insert_prob = stof(argv[9]);
    params.elite = 1;

    int top_sols_to_report = 5; //popsize; //<- to report all final solutions
    // Reading the instance file
    ifstream file_base(base_path);

    if(!file_base.is_open()){
        cout << "\nCannot open file: " << base_path << endl;
        exit(1);
    }

    cout << "Reading instance: " << base_path << "\n";
    readInstance(file_base);
    
    // Start the timer
    auto start = chrono::high_resolution_clock::now();

    // Initialize the population
    vector<individual> pop = initialize_pop();

    // Main loop of the evolutionary algorithm
    for (int i = 0; i < params.max_gen; i++)
    {      
        generateNewPop(pop);
        evaluatePop(pop);
        reportBest(i,pop,conv);
    }

    // End the timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;


    // Mostrar la mejor solución encontrada
    sort(pop.begin(), pop.end(), fitnessComparisonAsc);
    cout << "\nBest solution found:" << endl;
    writeInd(pop[0]);
    cout << "\nReporting top " << top_sols_to_report << " solutions..." << endl;
    reportSolutions(pop,solution);
    
    cout<<endl<<"Time elapsed: "<<elapsed.count()<<" seconds"<<endl;

    file_base.close();
    conv.close();
    solution.close();

    return 0;

}