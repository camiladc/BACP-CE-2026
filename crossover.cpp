#include <bits/stdc++.h>

#include "global.h"
using namespace std;

// When assigning course to period: period >= course_min_period[course]
// and isPlacementValid(course, period, assigned_period) after copying parent blocks.
// Use earliestFeasiblePeriod() for repair if a child violates prereq floors.

// Crossover functions
// Crossover 1:
// Select the best N semester from both parent as the base of the child, 
// then try to relocate the pending courses in the same semester as the other parent, 
// if not possible, create new semesters for them. 
// If a semester gets empty in the new child, remove it and move the following semesters one position up.
vector<individual> crossover(individual &parent1, individual &parent2){

}