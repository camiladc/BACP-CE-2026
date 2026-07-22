#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <fstream>

/* ------------- STRUCTURES ---------------------*/
struct individual
{
    // Individual representation: courses per semester
    std::vector<std::vector<int>> courses;
    // Tree-like structure equivalence of the representation for prerequisite tracking
    std::unordered_map<int,std::vector<std::pair<int,int>>> course_children_period;
    // feasibility flag
    bool is_feasible = true;

    // Individural fitness value
    float fitness[1];

    // Per semester fitness value
    std::vector<float> period_fitness;

    // Per semester credits load
    std::vector<int> total_credits_per_period;

};

struct hyperparams{
    int popsize;
    int max_gen;
    int elite;
    float cross_prob;
    float mut_prob;
    float mut_move_course_prob;
    float mut_branch_prob;
    float mut_insert_prob;
};

/* ------------- FUNCTION PROTOTYPES ---------------------*/

// Random functions
void randomize(int seed);
int getRandomInt(int a, int b);
float getRandomProb();
float getRandomProb_lim(float lim);

// Reader functions
void readInstance(std::ifstream &base_file);

// Evolutive Algorithm functions
individual initialize_ind();
std::vector<individual> initialize_pop();
bool fitnessComparisonAsc(const individual &a, individual &b);
void generateNewPop(std::vector<individual> &old_pop);
void evaluatePop(std::vector<individual> &pop);
individual selection(std::vector<individual> &old_pop);
std::vector<individual> crossover(individual &parent1, individual &parent2);
void mutation(individual &parent1);

// utility functions
std::vector<int> getRootCourses();
std::vector<int> getChildrenCourses(int id_course);
std::vector<int> getCourseParents(int id_course);
void buildCoursePeriodRelationship(individual& ind, int id_course);
void setCoursePeriodRelationship(individual& parent, individual& child, int id_course, std::vector<int>& courses_enrolled);
void saveBestPerPeriod(std::vector<individual>& pop, std::vector<individual>& best_per_period);

int getCoursePeriod(const individual& ind, int course);
int periodLoad(const individual& ind, int period, int skipCourse = -1);
int periodCourseCount(const individual& ind, int period, int skipCourse = -1);
int earliestFeasiblePeriod(int course, const individual& ind);
bool canPlaceCourse(const individual& ind, int course, int period, int skipCourse = -1);
bool validateIndividual(const individual& ind);
void syncCourseChildrenPeriod(individual& ind);
void collectBranchCourses(int root, std::vector<int>& out);
void removeCourseFromPeriod(individual& ind, int course, int period);
void trimTrailingEmptyPeriods(individual& ind);
void trimEmpyPeriods(individual& ind);
bool tryMoveCourse(individual& ind, int course, int fromPeriod, int toPeriod);

// Writer functions
void writeInd(individual &ind);
void reportBest(int gen, std::vector<individual>& pop, std::ofstream& out);
void reportSolutions(std::vector<individual>& pop, std::ofstream& out);
void reportBestPerPeriod(std::vector<individual>& best_per_period, std::ofstream& out);

/* ------------- GLOBAL VARIABLES ---------------------*/

// Hyperparams declaration
extern hyperparams params;

// Random number generator declaration
extern std::mt19937 rng;

// Instance data
extern int num_periods; //number of periods in the curriculum
extern int max_periods; // for maximal period definition
extern int min_load; // minimum academic load allowed per period
extern int max_load; // maximum academic load allowed per period
extern int min_courses; // minimum amount of courses allowed per period
extern int max_courses; // maximum amount of courses allowed per period
extern int num_courses; // number of courses in the curriculum
extern std::vector<std::string> course_names; // list of course names, indexed by course ID
extern std::vector<int> course_credits; // list of course credits, indexed by course ID
extern std::vector<std::pair<int,int>> prerequisites; // list of prerequisites, indexed by course ID
extern std::unordered_map<std::string,int> course_index; // map of course names to IDs
extern std::vector<std::vector<int>> prereq_adj; // adjacency list of prerequisites
#endif