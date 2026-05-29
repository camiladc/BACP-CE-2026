#include <bits/stdc++.h>

#include "global.h"
using namespace std;

// Moving a course earlier is valid only if isPlacementValid(course, new_period, assigned_period).
// After destructive moves, reassign with period >= earliestFeasiblePeriod(course, assigned_period).

// Mutation functions
// Destructive mutation: randomly remove a semester and reassign its courses to other semesters.
// Constructive mutation: create a new semester and move a random selection of courses from previous semesters.
// In both cases ensure that all constraints are still satisfied.
void mutation(individual &parent1){

}