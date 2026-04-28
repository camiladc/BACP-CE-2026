#include <bits/stdc++.h>
#include <random>

#include "global.h"
using namespace std;

//Get random number generator
void randomize(int seed){
    rng.seed(seed);
}

//Function that returns an integer between a and b inclusive
int getRandomInt(int a, int b) {
 //   cout << "VOY A LLORAR " << endl;
    uniform_int_distribution<int> dist(a, b);
//    cout << "NO llore" << endl;
    return dist(rng);
}

//Function that returns a real number between 0 and 1
float getRandomProb(){
    uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
}

//Function that returns a real number between 0 and lim
float getRandomProb_lim(float lim){
    uniform_real_distribution<float> dist(0.0f, lim);
    return dist(rng);
}