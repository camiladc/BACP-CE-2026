#include <algorithm>
#include<bits/stdc++.h>

#include "global.h"
using namespace std;

// Function to evaluate the fitness of each individual in the population
void evaluatePop(vector<individual> &pop){
    for (auto& ind: pop) {
        ind.is_feasible = true;
        // obj_period = (sum(CR_courses_period) - MEAN_credits_periods) ^ 2

        // get total credits per period
        const int total_periods = static_cast<int>(ind.courses.size());
        std::vector<int> total_credits_per_period(total_periods);
        for (int i = 0; i < total_periods; ++i) 
            total_credits_per_period[i] = std::accumulate(
                ind.courses[i].begin(), ind.courses[i].end(),
                0, [] (int sum, int cid) { return sum + course_credits[cid]; }
            );

        // get mean credits of current programming
        float MEAN_credits = std::accumulate(
            total_credits_per_period.begin(),
            total_credits_per_period.end(),
            (float) 0.0
        ) / num_periods;
        float surplus = MEAN_credits * MEAN_credits;
        
        ind.period_fitness.clear();
        ind.period_fitness.resize(total_periods);
        
        for (int i = 0; i < total_periods; i++) {
            float diff = total_credits_per_period[i] - MEAN_credits;
            ind.period_fitness[i] = diff * diff;
            if (i >= maxAllowedPeriods())
                ind.period_fitness[i] += surplus;
        }

        // final fitness is the total variance
        ind.fitness[0] = std::accumulate(ind.period_fitness.begin(), ind.period_fitness.end(), (float) 0.0);

        // assign course-period relationship in individual
        ind.course_children_period.clear();
        vector<int> root_courses = getRootCourses();
        for(const auto& course: course_names) {
            int id_course = course_index[course];
            buildCoursePeriodRelationship(ind, id_course);
        }
    }
}

// Function to sort individuals in ascending order of fitness
bool fitnessComparisonAsc(const individual &a, individual &b) {
    return a.fitness[0] < b.fitness[0];
}


// Roulette selection without replacement
individual selection(vector<individual> &old_pop){
    size_t pop_size = old_pop.size();
    
    // calculate selection probabilities
    std::vector<float> selection_probs(pop_size);
    float total_fit = std::accumulate(
        old_pop.begin(), old_pop.end(),
        (float) 0.0, [] (float sum, const individual& ind) {
            return sum + ind.fitness[0];
        }
    );

    std::transform(
        old_pop.begin(), old_pop.end(),
        selection_probs.begin(), [&total_fit] (const individual& ind) {
            return ind.fitness[0] / total_fit;
        }
    );

    // for minimization, inverse the probabilities and normalize so that its sum is 1
    std::transform(
        selection_probs.begin(), selection_probs.end(),
        selection_probs.begin(), [&pop_size] (float p) {
            return (1 - p) / (pop_size - 1);
        }
    );

    float r = getRandomProb();
    float cum_prob = 0.0;

    for (int i = 0; i < static_cast<int>(old_pop.size()); ++i) {
        cum_prob += selection_probs[i];
        if (cum_prob >= r && old_pop[i].is_feasible) 
            return old_pop[i];
    }

    /*
    default: return individual with best fitness that is feasible
    @note: is this the best way to select an individual if the previous loop fails?
    */
    for (int i = 0; i < static_cast<int>(selection_probs.size()); i++)
        if (!old_pop[i].is_feasible)
            selection_probs[i] = 0.0F;
    

    auto max_it = std::max_element(selection_probs.begin(), selection_probs.end());
    size_t best = std::distance(selection_probs.begin(), max_it);

    return old_pop[best];
}

// Function to generate a new population from the old population using selection, crossover, and mutation
void generateNewPop(vector<individual> &old_pop){

    vector<int> a1(params.popsize+1), a2(params.popsize+1);
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
        if(p <= params.cross_prob){
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
        else if(p <= (params.cross_prob + params.mut_prob)){

            mutation(parent1);

            new_pop[c] = parent1;
        }
        else
            new_pop[c] = parent1;
    }
    old_pop = new_pop;
}
