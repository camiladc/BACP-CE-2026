#include <bits/stdc++.h>

#include "global.h"
using namespace std;

int earliestFeasiblePeriod(int course, const vector<int> &assigned_period) {
    int earliest = (course >= 0 && course < static_cast<int>(course_min_period.size()))
        ? course_min_period[course] : 0;
    if (course >= 0 && course < num_courses) {
        for (int prereq : prereq_adj[course]) {
            if (prereq >= 0 && prereq < num_courses && assigned_period[prereq] >= 0) {
                earliest = max(earliest, assigned_period[prereq] + 1);
            }
        }
    }
    return earliest;
}

bool isPlacementValid(int course, int period, const vector<int> &assigned_period) {
    return period >= earliestFeasiblePeriod(course, assigned_period);
}

individual initialize_ind(){

    individual new_ind;
    new_ind.courses.clear();
    new_ind.fitness[0] = 0.0;
    new_ind.period_fitness.clear();

    // Build prerequisite counters and dependent lists for each course
    // remaining_prereqs[c] is the number of prereqs still unscheduled for course c
    // dependents[p] lists all courses that depend on course p
    vector<int> remaining_prereqs(num_courses, 0);
    vector<vector<int>> dependents(num_courses);
    for (const auto &pr : prerequisites) {
        if (pr.first >= 0 && pr.first < num_courses && pr.second >= 0 && pr.second < num_courses) {
            remaining_prereqs[pr.first]++;
            dependents[pr.second].push_back(pr.first);
        }
    }

    // Initialize the ready list with courses that have no prerequisites
    vector<int> ready;
    ready.reserve(num_courses);
    for (int course = 0; course < num_courses; ++course) {
        if (remaining_prereqs[course] == 0) {
            ready.push_back(course);
        }
    }

    // Track load and course count for each generated semester
    vector<int> period_load;
    vector<int> period_count;
    vector<int> assigned_period(num_courses, -1);

    // Helper to safely read credit values from the instance data.
    auto courseCredit = [&](int course) {
        return (course >= 0 && course < static_cast<int>(course_credits.size())) ? course_credits[course] : 0;
    };

    // Scheduling loop: evaluate ready courses, choose randomly among the
    // best N candidates, and then place the chosen course in the earliest
    // feasible semester slot.
    while (!ready.empty()) {

        int pickIndex = getRandomInt(0, ready.size()-1);
        int course = ready[pickIndex];

        auto it = find(ready.begin(), ready.end(), course);
        if (it != ready.end()) {
            ready.erase(it);
        }

        int earliest_period = earliestFeasiblePeriod(course, assigned_period);

        int credit = courseCredit(course);
        int best_period = -1;

        // Ensure there are enough semester slots to consider the earliest feasible period
        if (earliest_period >= static_cast<int>(new_ind.courses.size())) {
            new_ind.courses.resize(earliest_period + 1);
            period_load.resize(earliest_period + 1, 0);
            period_count.resize(earliest_period + 1, 0);
        }

        // Try to place the course in an existing semester with room
        for (int period = earliest_period; period < static_cast<int>(new_ind.courses.size()); ++period) {
            if (period_count[period] < max_courses && period_load[period] + credit <= max_load) {
                best_period = period;
                break;
            }
        }

        // If no existing semester fits, create a new semester
        if (best_period == -1) {
            new_ind.courses.emplace_back();
            period_load.push_back(0);
            period_count.push_back(0);
            best_period = static_cast<int>(new_ind.courses.size()) - 1;
        }

        // Assign the course to the selected semester
        new_ind.courses[best_period].push_back(course);
        assigned_period[course] = best_period;
        period_count[best_period]++;
        period_load[best_period] += credit;

        // Add newly available courses to the ready list
        for (int child : dependents[course]) {
            if (--remaining_prereqs[child] == 0) {
                ready.push_back(child);
            }
        }
    }

    return new_ind;
}

vector<individual> initialize_pop(){
    
    vector<individual> pop = {};
    for(int i = 0; i < params.popsize; i++){
        individual new_ind = initialize_ind();
        pop.push_back(new_ind);

    }
    evaluatePop(pop);

    return pop;
}