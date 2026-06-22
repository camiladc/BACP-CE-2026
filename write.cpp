#include <bits/stdc++.h>

#include "global.h"
using namespace std;

void writeInd(individual &ind){
    cout << "Solution summary:" << endl;
    cout << "  Feasible: " << (ind.is_feasible ? "yes" : "no") << endl;
    cout << "  Fitness: " << ind.fitness[0] << endl;
    cout << "  Periods: " << ind.courses.size() << endl;

    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        cout << "  Period " << p << ":";
        if (ind.courses[p].empty()) {
            cout << " (empty)";
        } else {
            for (int course : ind.courses[p]) {
                if (course >= 0 && course < static_cast<int>(course_names.size())) {
                    cout << " " << course_names[course];
                } else {
                    cout << " #" << course;
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}
