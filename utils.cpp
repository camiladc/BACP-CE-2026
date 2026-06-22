#include <bits/stdc++.h>

#include "global.h"
using namespace std;

// function to get courses that have completed prerequisites for the students remaining courses
vector<int> getRootCourses() {
    vector<int> root_courses;
    root_courses.clear();
    for (const auto& course: remaining_courses) {
        int is_root = 1;
        // for each prerequisite of the course...
        for(const auto& prereq: prereq_adj[course_index[course]]) {
            // if the student has yet to attend the prerequisite, then the course is not a root..
            if (find(remaining_courses.begin(), remaining_courses.end(), course_names[prereq]) != remaining_courses.end()){
                is_root = 0;
                break;
            }
        }

        if (is_root)
            root_courses.push_back(course_index[course]);
    }

    return root_courses;
}

// function to get courses that have id_course as prerequisite and the student has yet to attend
vector<int> getChildrenCourses(int id_course) {
    vector<int> children;
    children.clear();

    for (const auto& course: remaining_courses) {
        if (find(prereq_adj[course_index[course]].begin(), prereq_adj[course_index[course]].end(), id_course) != prereq_adj[course_index[course]].end()) {
            children.push_back(course_index[course]);
        }
    }
    return children;
}

//function to get parents for child course in student reamining courses
vector<int> getCourseParents(int id_course) {
    vector<int> parents;
    parents.clear();

    for(const auto& parent : prereq_adj[id_course]) {
        if (find(remaining_courses.begin(), remaining_courses.end(), course_names[parent]) != remaining_courses.end()) {
            parents.push_back(parent);
        }
    }

    return parents;
}

// function to construct course-period structure for an individual
void buildCoursePeriodRelationship(individual& ind, int id_course) {
    int p = 0;

    for (int _p = 0; _p < static_cast<int>(ind.courses.size()); _p++){
        if (find(ind.courses[_p].begin(), ind.courses[_p].end(), id_course) == ind.courses[_p].end())
            continue;
        p = _p;
        break;
    }

    auto _children = getChildrenCourses(id_course);
    if (ind.course_children_period.find(id_course) == ind.course_children_period.end()) {
        ind.course_children_period[id_course].clear();
        ind.course_children_period[id_course].resize(_children.size()+1);
        ind.course_children_period[id_course][0] = make_pair(id_course, p);
    }

    int parent_period = p;
    for(auto& child_course : _children) {
        for (int _p = 0; _p < static_cast<int>(ind.courses.size()); _p++){
            if (find(ind.courses[_p].begin(), ind.courses[_p].end(), child_course) == ind.courses[_p].end())
                continue;
            p = _p;
            break;
        }

        // if children is placed before or in the same period as the parent, the individual is not feasible
        if (p <= parent_period)
            ind.is_feasible = false;

        ind.course_children_period[id_course].push_back(make_pair(child_course, p));

        // recursively build children relationships if they haven't been built yet
        if (ind.course_children_period.find(child_course) == ind.course_children_period.end()) {
            buildCoursePeriodRelationship(ind, child_course);
        }
    }
}

// function to assign each course to the respective period in a given branch
void setCoursePeriodRelationship(individual& parent, individual& child, int id_course, vector<int>& courses_enrolled) {
    if (find(courses_enrolled.begin(), courses_enrolled.end(), id_course) != courses_enrolled.end())
        return;

    auto addEnrolled = [&](int course) {
        if (find(courses_enrolled.begin(), courses_enrolled.end(), course) == courses_enrolled.end())
            courses_enrolled.push_back(course);
    };

    auto ensureRelationship = [&](int course) {
        auto it = parent.course_children_period.find(course);
        if (it == parent.course_children_period.end() || it->second.empty()) {
            buildCoursePeriodRelationship(parent, course);
        }
    };

    auto _parents = getCourseParents(id_course);
    auto _children = getChildrenCourses(id_course);

    ensureRelationship(id_course);

    for (auto& parent_course : _parents) {
        ensureRelationship(parent_course);
        int p = parent.course_children_period[parent_course][0].second;
        if (p >= static_cast<int>(child.courses.size()))
            child.courses.resize(p + 1);
        if (find(child.courses[p].begin(), child.courses[p].end(), parent_course) == child.courses[p].end()) {
            child.courses[p].push_back(parent_course);
        }
        addEnrolled(parent_course);
        setCoursePeriodRelationship(parent, child, parent_course, courses_enrolled);
    }

    for (auto& child_course: _children) {
        ensureRelationship(child_course);
        int p = parent.course_children_period[child_course][0].second;
        if (p >= static_cast<int>(child.courses.size()))
            child.courses.resize(p + 1);
        if (find(child.courses[p].begin(), child.courses[p].end(), child_course) == child.courses[p].end()){
            child.courses[p].push_back(child_course);
        }
        addEnrolled(child_course);
        setCoursePeriodRelationship(parent, child, child_course, courses_enrolled);
    }
}

static bool isRemainingCourseId(int course) {
    return find(remaining_courses.begin(), remaining_courses.end(), course_names[course])
        != remaining_courses.end();
}

int maxAllowedPeriods() {
    return static_cast<int>(lround(num_periods * 2.5));
}

int getCoursePeriod(const individual& ind, int course) {
    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        if (find(ind.courses[p].begin(), ind.courses[p].end(), course) != ind.courses[p].end())
            return p;
    }
    return -1;
}

int periodLoad(const individual& ind, int period, int skipCourse) {
    if (period < 0 || period >= static_cast<int>(ind.courses.size()))
        return 0;
    int load = 0;
    for (int cid : ind.courses[period]) {
        if (cid == skipCourse)
            continue;
        load += (cid >= 0 && cid < static_cast<int>(course_credits.size())) ? course_credits[cid] : 0;
    }
    return load;
}

int periodCourseCount(const individual& ind, int period, int skipCourse) {
    if (period < 0 || period >= static_cast<int>(ind.courses.size()))
        return 0;
    int count = 0;
    for (int cid : ind.courses[period]) {
        if (cid == skipCourse)
            continue;
        ++count;
    }
    return count;
}

int earliestFeasiblePeriod(int course, const individual& ind) {
    int earliest = 0;
    if (course < 0 || course >= num_courses)
        return 0;

    for (int prereq : prereq_adj[course]) {
        if (!isRemainingCourseId(prereq))
            continue;
        int prereqPeriod = getCoursePeriod(ind, prereq);
        if (prereqPeriod < 0)
            return maxAllowedPeriods();
        earliest = max(earliest, prereqPeriod + 1);
    }
    return earliest;
}

bool canPlaceCourse(const individual& ind, int course, int period, int skipCourse) {
    if (course < 0 || course >= num_courses)
        return false;
    if (period < 0 || period >= static_cast<int>(ind.courses.size()))
        return false;
    if (period < earliestFeasiblePeriod(course, ind))
        return false;

    int credit = (course >= 0 && course < static_cast<int>(course_credits.size())) ? course_credits[course] : 0;
    if (periodLoad(ind, period, skipCourse) + credit > max_load)
        return false;
    if (periodCourseCount(ind, period, skipCourse) + 1 > max_courses)
        return false;

    for (int child : getChildrenCourses(course)) {
        int childPeriod = getCoursePeriod(ind, child);
        if (childPeriod >= 0 && childPeriod <= period)
            return false;
    }

    return true;
}

bool validateIndividual(const individual& ind) {
    if (static_cast<int>(ind.courses.size()) > maxAllowedPeriods())
        return false;

    for (const auto& courseName : remaining_courses) {
        int course = course_index[courseName];
        if (getCoursePeriod(ind, course) < 0)
            return false;
    }

    for (int p = 0; p < static_cast<int>(ind.courses.size()); ++p) {
        if (periodLoad(ind, p) > max_load)
            return false;
        if (periodCourseCount(ind, p) > max_courses)
            return false;
    }

    for (const auto& courseName : remaining_courses) {
        int course = course_index[courseName];
        int coursePeriod = getCoursePeriod(ind, course);
        for (int prereq : prereq_adj[course]) {
            if (!isRemainingCourseId(prereq))
                continue;
            int prereqPeriod = getCoursePeriod(ind, prereq);
            if (prereqPeriod < 0 || coursePeriod <= prereqPeriod)
                return false;
        }
    }

    return true;
}

void syncCourseChildrenPeriod(individual& ind) {
    ind.is_feasible = true;
    ind.course_children_period.clear();
    for (const auto& course : remaining_courses) {
        buildCoursePeriodRelationship(ind, course_index[course]);
    }
}

void collectBranchCourses(int root, vector<int>& out) {
    out.clear();
    vector<int> stack = {root};
    vector<int> visited;

    while (!stack.empty()) {
        int course = stack.back();
        stack.pop_back();
        if (find(visited.begin(), visited.end(), course) != visited.end())
            continue;
        visited.push_back(course);
        out.push_back(course);
        for (int child : getChildrenCourses(course))
            stack.push_back(child);
    }
}

void removeCourseFromPeriod(individual& ind, int course, int period) {
    if (period < 0 || period >= static_cast<int>(ind.courses.size()))
        return;
    auto& periodCourses = ind.courses[period];
    auto it = find(periodCourses.begin(), periodCourses.end(), course);
    if (it != periodCourses.end())
        periodCourses.erase(it);
}

void trimTrailingEmptyPeriods(individual& ind) {
    while (!ind.courses.empty() && ind.courses.back().empty())
        ind.courses.pop_back();
}