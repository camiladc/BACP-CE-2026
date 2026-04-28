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

    if(argc < 5){
        cout << "Usage:\n./main instance_path seed max_gen popsize cross_prob mut_prob \n";
        exit(1);
    }

    //Asignamos el path
    string path = argv[1];
  
    //Asignamos la semilla 
    int seed = atoi(argv[2]);
    randomize(seed);

    //Definimos hyperparametros del algoritmo
    params.max_gen = atoi(argv[3]);
    params.popsize = atoi(argv[4]);
    params.cross_prob = stof(argv[5]);
    params.mut_prob = stof(argv[6]);
    params.elite = 1;
    
    //Comenzamos lectura de la instancia
    ifstream file(path);

    if(!file.is_open()){
        cout << "\nCannot open file: " << path << endl;
        exit(1);
    }

    cout << "Reading instance: " << path << "\n";
    readInstance(file);
    
    auto start = chrono::high_resolution_clock::now();

    vector<individual> pop = initialize_pop();

    for (int i = 0; i < params.max_gen; i++)
    {      
        generateNewPop(pop);
        evaluatePop(pop);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout<<endl<<"Time elapsed: "<<elapsed.count()<<" seconds"<<endl;

    return 0;

}