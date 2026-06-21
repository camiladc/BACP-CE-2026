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

    for (int _p = 0; _p < ind.courses.size(); _p++){
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
        for (int _p = 0; _p < ind.courses.size(); _p++){
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
    auto _parents = getCourseParents(id_course);
    auto _children = getChildrenCourses(id_course);

    for (auto& parent_course : _parents) {
        int p = parent.course_children_period[parent_course][0].second;
        if (find(child.courses[p].begin(), child.courses[p].end(), parent_course) == child.courses[p].end()) {
            child.courses[p].push_back(parent_course);
            courses_enrolled.push_back(parent_course);
        }

        setCoursePeriodRelationship(parent, child, parent_course, courses_enrolled);
    }

    for (auto& child_course: _children) {
        int p = parent.course_children_period[child_course][0].second;
        if (find(child.courses[p].begin(), child.courses[p].end(), child_course) == child.courses[p].end()){
            child.courses[p].push_back(child_course);
            courses_enrolled.push_back(child_course);
        }
        setCoursePeriodRelationship(parent, child, child_course, courses_enrolled);
    }
}