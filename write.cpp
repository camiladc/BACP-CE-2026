#include <bits/stdc++.h>
#include <fstream>

#include "global.h"
using namespace std;

void writeInd(individual &ind){
    cout << "Solution summary:" << endl;
    cout << "  Feasible: " << (ind.is_feasible ? "yes" : "no") << endl;
    cout << "  Fitness: " << ind.fitness[0] << endl;
    cout << "  Periods: " << ind.courses.size() << endl;

    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        if (ind.courses[p].empty()) {
            cout << "  Period " << p << " (empty)";
            cout << endl;
            continue;
        }

        int period_total_credits = 0;
        for (int course : ind.courses[p]) {
            if (course >= 0 && course < static_cast<int>(course_credits.size())) {
                period_total_credits += course_credits[course];
            }
        }

        cout << "  Period " << p << " - " << period_total_credits << " credits:";
        for (int course : ind.courses[p]) {
            if (course >= 0 && course < static_cast<int>(course_names.size())) {
                cout << " " << course_names[course];
            } else {
                cout << " #" << course;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void reportBest(int gen, vector<individual>& pop, ofstream& out) {
    vector<individual> aux_pop = pop;

    sort(aux_pop.begin(), aux_pop.end(), fitnessComparisonAsc);
    individual best;
    for (const individual& ind : aux_pop) {
        if (ind.is_feasible){
            best = ind;
            break;
        }
    }
    out << gen << ";" << best.fitness[0] << endl;
}

void reportSolutions(vector<individual>& pop, ofstream& out) {
    vector<individual> aux_pop = pop;
    sort(aux_pop.begin(), aux_pop.end(), fitnessComparisonAsc);
    
    for (int t = 0; t < static_cast<int>(pop.size()); t++) {
        individual ind = aux_pop[t];
        for(int p = 0; p < static_cast<int>(ind.courses.size()); p++) {
            out << t+1 << ";" << p+1 << ";";
            for (int i = 0; i < static_cast<int>(ind.courses[p].size()); i++)
                out << course_names[ind.courses[p][i]] << ",";
            out << ";";
            int total_credits = accumulate(
                ind.courses[p].begin(), ind.courses[p].end(),
                0, [] (int sum, int cid) { return sum + course_credits[cid]; }
            );
            out << total_credits << ";" << ind.period_fitness[p] << ";" << ind.fitness[0] << endl;
        }
    }
}