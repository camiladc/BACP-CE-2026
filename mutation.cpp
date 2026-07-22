#include <bits/stdc++.h>

#include "global.h"
using namespace std;

static bool mutateBranchShift(individual& ind) {
    syncCourseChildrenPeriod(ind);

    vector<int> roots = getRootCourses();
    if (roots.empty())
        return false;

    int root = roots[getRandomInt(0, static_cast<int>(roots.size()) - 1)];

    vector<int> branch;
    collectBranchCourses(root, branch);

    vector<int> candidates;
    for (int course : branch) {
        if (course != root)
            candidates.push_back(course);
    }
    if (candidates.empty())
        return false;

    int course = candidates[getRandomInt(0, static_cast<int>(candidates.size()) - 1)];
    int currentPeriod = getCoursePeriod(ind, course);
    if (currentPeriod < 0)
        return false;

    int delta = (getRandomProb() <= 0.5f) ? -1 : 1;
    int newPeriod = currentPeriod + delta;
    if (newPeriod < 0)
        return false;

    if (newPeriod >= static_cast<int>(ind.courses.size())) {
        if (static_cast<int>(ind.courses.size()) >= max_periods)
            return false;
        ind.courses.resize(newPeriod + 1);
    }

    if (!canPlaceCourse(ind, course, newPeriod, course))
        return false;

    removeCourseFromPeriod(ind, course, currentPeriod);
    ind.courses[newPeriod].push_back(course);
    trimEmpyPeriods(ind);

    return validateIndividual(ind);
}

static bool mutateInsertPeriod(individual& ind) {
    if (static_cast<int>(ind.courses.size()) >= max_periods)
        return false;
    if (ind.courses.empty())
        return false;

    int k = getRandomInt(0, static_cast<int>(ind.courses.size()));
    ind.courses.insert(ind.courses.begin() + k, vector<int>{});

    vector<int> sourceOptions;
    if (k - 1 >= 0)
        sourceOptions.push_back(k - 1);
    if (k + 1 < static_cast<int>(ind.courses.size()))
        sourceOptions.push_back(k + 1);
    if (sourceOptions.empty())
        return false;

    int source = sourceOptions[getRandomInt(0, static_cast<int>(sourceOptions.size()) - 1)];
    if (ind.courses[source].empty())
        return false;

    int maxMoves = max(1, static_cast<int>(ind.courses[source].size()) / 2);
    int movesToTry = getRandomInt(1, maxMoves);

    vector<int> sourceCourses = ind.courses[source];
    shuffle(sourceCourses.begin(), sourceCourses.end(), rng);

    int moved = 0;
    for (int cid : sourceCourses) {
        if (moved >= movesToTry)
            break;
        int fromPeriod = getCoursePeriod(ind, cid);
        if (fromPeriod < 0)
            continue;
        if (tryMoveCourse(ind, cid, fromPeriod, k))
            ++moved;
    }

    trimEmpyPeriods(ind);
    if (moved == 0)
        return false;

    return validateIndividual(ind);
}

static bool findFeasiblePeriodAny(const individual& ind, int course, int& outPeriod) {
    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        if (canPlaceCourse(ind, course, p, course)) {
            outPeriod = p;
            return true;
        }
    }
    return false;
}

static bool mutateRemovePeriod(individual& ind) {
    if (ind.courses.size() <= 1)
        return false;

    vector<int> nonEmptyPeriods;
    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        if (!ind.courses[p].empty())
            nonEmptyPeriods.push_back(p);
    }
    if (nonEmptyPeriods.empty())
        return false;

    int k = nonEmptyPeriods[getRandomInt(0, static_cast<int>(nonEmptyPeriods.size()) - 1)];
    vector<int> coursesToMove = ind.courses[k];

    ind.courses.erase(ind.courses.begin() + k);

    for (int course : coursesToMove) {
        int targetPeriod = -1;
        if (!findFeasiblePeriodAny(ind, course, targetPeriod))
            return false;
        ind.courses[targetPeriod].push_back(course);
    }

    trimEmpyPeriods(ind);
    return validateIndividual(ind);
}

// Function to move a course from one period with greater credits to another with less, ensuring feasibility
static bool mutateMoveCourseLowerCreditPeriod(individual& ind) {
    if (ind.courses.size() <= 1)
        return false;

    // Get the period with maximum credits using total_credits_per_period
    int maxCreditsPeriod = std::distance(ind.total_credits_per_period.begin(),
        std::max_element(ind.total_credits_per_period.begin(), ind.total_credits_per_period.end()));    

    // Get the period with minimun credits using total_credits_per_period
    int minCreditsPeriod = std::distance(ind.total_credits_per_period.begin(),
        std::min_element(ind.total_credits_per_period.begin(), ind.total_credits_per_period.end()));
  
    // Select a random course from the maxCreditsPeriod to move it to the minCreditsPeriod
    int courseToMove = ind.courses[maxCreditsPeriod][getRandomInt(0, static_cast<int>(ind.courses[maxCreditsPeriod].size()) - 1)];    

    // Attempt to move the selected course to the minCreditsPeriod
    if (!tryMoveCourse(ind, courseToMove, maxCreditsPeriod, minCreditsPeriod))
        return false;

    trimEmpyPeriods(ind);
    return validateIndividual(ind);
}

void mutation(individual& parent1) {
    individual backup = parent1;
    bool ok = false;
    float r = getRandomProb();

    if (r <= params.mut_move_course_prob)
        ok = mutateMoveCourseLowerCreditPeriod(parent1);
    else if (r <= (params.mut_branch_prob + params.mut_move_course_prob))
        ok = mutateBranchShift(parent1);
    else if (r <= (params.mut_branch_prob + params.mut_move_course_prob + params.mut_insert_prob))
        ok = mutateInsertPeriod(parent1);
    else
        ok = mutateRemovePeriod(parent1);

    if (!ok)
        parent1 = backup;
}
