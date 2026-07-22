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

// Random number generator declaration
mt19937 rng;

int main(int argc, char *argv[]){

    if(argc < 10){
        cout << "Usage:\n./main base_instance_path seed max_gen popsize cross_prob mut_prob mut_move_course_prob mut_branch_prob mut_insert_prob \n";
        cout << "Example:\n./main benchmark-instances/bacp7_diego.dat 13 800 25 0.2 0.7 0.2 0.6 0.1 \n";
        exit(1);
    }

    // Assign the path
    // inputs
    string base_path = argv[1];

    // Assign the seed
    int seed = atoi(argv[2]);
    randomize(seed);

    //outputs for instance and seed
    string stem = fs::path(base_path).stem().string();
    string f_convergence = format("convergence_results/convergence_{}_{}.csv", stem, seed); // convergence graph
    string f_sol = format("solutions/solutions_{}_{}.csv", stem, seed); // formated solution as csv
    string period_sol = format("dynamics_solutions/best_per_periods_{}_{}.csv", stem, seed);
    
    // create output directories if they don't exist
    fs::create_directories("convergence_results");
    fs::create_directories("solutions");
    fs::create_directories("dynamics_solutions");

    ofstream conv(f_convergence, ios::out);
    ofstream solution(f_sol, ios::out);
    ofstream period_solution(period_sol, ios::out);

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

    if(period_solution.is_open())
    {
        period_solution.flush();
    }
    else
    {
        cerr << "Failed to create file" << period_sol << ": " << errno << endl;
    }
    
    conv << "# gen;best_fit\n";
    solution << "id_sol;period;courses;total_credits;period_fit;total_fit\n";
    period_solution << "total_periods;period;courses;total_credits;period_fit;total_fit\n";

    // Define the hyperparameters
    params.max_gen = atoi(argv[3]);
    params.popsize = atoi(argv[4]);
    params.cross_prob = stof(argv[5]);
    params.mut_prob = stof(argv[6]);
    params.mut_move_course_prob = stof(argv[7]);
    params.mut_branch_prob = stof(argv[8]);
    params.mut_insert_prob = stof(argv[9]);
    params.elite = 1;

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

    // Initialize the best individual per period vector
    vector<individual> best_per_period;

    // Main loop of the evolutionary algorithm
    for (int i = 0; i < params.max_gen; i++)
    {      
        generateNewPop(pop);
        evaluatePop(pop);
        reportBest(i,pop,conv);
        saveBestPerPeriod(pop, best_per_period);
    }

    // End the timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;


    // Mostrar la mejor solución encontrada
    sort(pop.begin(), pop.end(), fitnessComparisonAsc);
    cout << "\nBest solution found:" << endl;
    writeInd(pop[0]);
    cout << "\nReporting top " << params.popsize << " solutions..." << endl;
    reportSolutions(pop, solution);

    reportBestPerPeriod(best_per_period, period_solution);
    
    cout<<endl<<"Time elapsed: "<<elapsed.count()<<" seconds"<<endl;

    file_base.close();
    conv.close();
    solution.close();
    period_solution.close();
    return 0;

}