#include <bits/stdc++.h>

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
