#include <bits/stdc++.h>

#include "global.h"
using namespace std;

// Crossover functions
void inherit(individual& parent1, individual& parent2, individual& child, int root_course) {
    child.courses.resize(parent2.courses.size());
    vector<int> courses_enrolled;
    setCoursePeriodRelationship(parent2, child, root_course, courses_enrolled);

    vector<int> total_courses, courses_not_enrolled;
    for(const auto& course: remaining_courses)
        total_courses.push_back(course_index[course]);
    
    // courses to inherit from p1 is the set TotalCourses \ EnrolledCourses
    std::set_difference(
        total_courses.begin(), total_courses.end(),
        courses_enrolled.begin(), courses_enrolled.end(),
        back_inserter(courses_not_enrolled)
    );

    // temporal copy of p1
    individual _tmp_p1 = parent1; 

    for (const auto& id_course : courses_not_enrolled) {
        int p = _tmp_p1.course_children_period[id_course][0].second;
        if (p > child.courses.size()-1)
            _tmp_p1.course_children_period[id_course][0].second = child.courses.size()-1;
    }

    for (const auto& id_course : courses_not_enrolled) {
        setCoursePeriodRelationship(_tmp_p1, child, id_course, courses_enrolled);
    }

}

// inherit random prereq tree between two parents, ensuring that all constraints are still satisfied
vector<individual> crossover(individual &parent1, individual &parent2){
    // select random course and 
    vector<int> root_courses = getRootCourses(), sampled;
    
    sample(root_courses.begin(), root_courses.end(), back_inserter(sampled), 1, rng);
    int sampled_course = sampled[0];
    
    individual child1, child2;
    child1.course_children_period.clear();
    child1.courses.clear();
    child2.course_children_period.clear();
    child2.courses.clear();

    // ch1 inherits subgraph from p2, the rest is inherited from p1
    inherit(parent1, parent2, child1, sampled_course);
    // ch2 inherits subgraph from p1, the rest is inherited from p2
    inherit(parent2, parent1, child2, sampled_course);

    return vector<individual> {child1, child2};
}