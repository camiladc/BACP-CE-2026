#include <bits/stdc++.h>
#include <fstream>

#include "global.h"
using namespace std;

int num_periods = 0;
int min_load = 0;
int max_load = 0;
int min_courses = 0;
int max_courses = 0;
int num_courses = 0;
vector<string> course_names;
vector<int> course_credits;
vector<pair<int,int>> prerequisites;
unordered_map<string,int> course_index;
vector<vector<int>> prereq_adj;
vector<int> course_min_period;

static void computeCourseMinPeriod() {
    vector<vector<int>> dependents(num_courses);
    vector<int> indegree(num_courses, 0);

    for (const auto &pr : prerequisites) {
        int course = pr.first;
        int prereq = pr.second;
        if (course >= 0 && course < num_courses && prereq >= 0 && prereq < num_courses) {
            dependents[prereq].push_back(course);
            indegree[course]++;
        }
    }

    course_min_period.assign(num_courses, 0);
    queue<int> q;
    int processed = 0;

    for (int i = 0; i < num_courses; ++i) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed++;

        for (int v : dependents[u]) {
            course_min_period[v] = max(course_min_period[v], course_min_period[u] + 1);
            if (--indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (processed < num_courses) {
        cerr << "Warning: prerequisite graph contains cycles; course_min_period may be incomplete." << endl;
    }
}

// Remove leading and trailing whitespace from a string
static string trim(const string &s) {
    // Move past leading whitespace characters
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) start++;

    // Move back from the end past trailing whitespace characters
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) end--;

    // Return the trimmed substring
    return s.substr(start, end - start);
}

// Find the matching closing delimiter for a nested block
static size_t findMatchingDelimiter(const string &text, size_t openPos, char openDelim, char closeDelim) {
    int depth = 0;
    for (size_t i = openPos; i < text.size(); ++i) {
        // Increase nesting depth for each opening delimiter
        if (text[i] == openDelim) depth++;
        else if (text[i] == closeDelim) {
            // Decrease depth when a closing delimiter is found
            depth--;
            if (depth == 0) return i;
        }
    }
    // Return npos if no matching closing delimiter is found
    return string::npos;
}


// The function reads the instance file, and extracts the
// scalar parameters p, a, b, c, d as well as the course list, credit list,
// and prerequisite pairs
void readInstance(ifstream &f){
    // Read the entire file into a single string so we can parse it flexibly
    string content;
    string line;
    while (getline(f, line)) {
        content += line;
        content.push_back('\n');
    }

    // Locate assignment statements like "p=", "a=", "courses =", etc
    auto findAssignment = [&](const string &key) {
        size_t pos = 0;
        while (true) {
            pos = content.find(key, pos);
            if (pos == string::npos) return string::npos;
            if (pos + key.size() >= content.size() || content[pos + key.size()] != '=') {
                pos += key.size();
                continue;
            }
            if (pos > 0 && (isalnum((unsigned char)content[pos - 1]) || content[pos - 1] == '_')) {
                pos += key.size();
                continue;
            }
            return pos;
        }
    };

    // Parse a single integer value from an assignment
    auto parseInt = [&](const string &key, int &value) {
        size_t pos = findAssignment(key);
        if (pos == string::npos) return false;
        pos = content.find('=', pos) + 1;
        while (pos < content.size() && isspace((unsigned char)content[pos])) pos++;
        size_t end = pos;
        if (end < content.size() && (content[end] == '+' || content[end] == '-')) end++;
        while (end < content.size() && isdigit((unsigned char)content[end])) end++;
        if (end == pos) return false;
        value = stoi(content.substr(pos, end - pos));
        return true;
    };

    // Parse a delimited block such as courses { ... }, credit [ ... ], or prereq { ... }
    auto parseBlock = [&](const string &key, char openDelim, char closeDelim, string &out) {
        size_t pos = findAssignment(key);
        if (pos == string::npos) return false;
        pos = content.find(openDelim, pos);
        if (pos == string::npos) return false;
        size_t closePos = findMatchingDelimiter(content, pos, openDelim, closeDelim);
        if (closePos == string::npos) return false;
        out = content.substr(pos + 1, closePos - pos - 1);
        return true;
    };

    // Read the scalar instance parameters
    if (!parseInt("p", num_periods)) {
        cerr << "Error: could not read number of periods (p)." << endl;
    }
    if (!parseInt("a", min_load)) {
        cerr << "Error: could not read minimum load (a)." << endl;
    }
    if (!parseInt("b", max_load)) {
        cerr << "Error: could not read maximum load (b)." << endl;
    }
    if (!parseInt("c", min_courses)) {
        cerr << "Error: could not read minimum course load (c)." << endl;
    }
    if (!parseInt("d", max_courses)) {
        cerr << "Error: could not read maximum course load (d)." << endl;
    }

    // Read the course name list from the courses block
    string block;
    course_names.clear();
    if (parseBlock("courses", '{', '}', block)) {
        string token;
        stringstream ss(block);
        while (getline(ss, token, ',')) {
            string name = trim(token);
            if (!name.empty()) course_names.push_back(name);
        }
    } else {
        cerr << "Error: could not read courses list." << endl;
    }

    // Read the credit list from the credit block
    course_credits.clear();
    if (parseBlock("credit", '[', ']', block)) {
        string token;
        stringstream ss(block);
        while (getline(ss, token, ',')) {
            string number = trim(token);
            if (!number.empty()) course_credits.push_back(stoi(number));
        }
    } else {
        cerr << "Error: could not read credit list." << endl;
    }

    // Build a name-to-index map for fast prereq lookup
    course_index.clear();
    for (size_t i = 0; i < course_names.size(); ++i) {
        course_index[course_names[i]] = static_cast<int>(i);
    }

    // Parse prerequisite pairs from the prereq block
    prerequisites.clear();
    if (parseBlock("prereq", '{', '}', block)) {
        size_t pos = 0;
        while (true) {
            pos = block.find('<', pos);
            if (pos == string::npos) break;
            size_t closePos = block.find('>', pos);
            if (closePos == string::npos) break;

            string pairText = block.substr(pos + 1, closePos - pos - 1);
            size_t commaPos = pairText.find(',');
            if (commaPos != string::npos) {
                string course = trim(pairText.substr(0, commaPos));
                string prereq = trim(pairText.substr(commaPos + 1));
                auto courseIt = course_index.find(course);
                auto prereqIt = course_index.find(prereq);
                if (courseIt != course_index.end() && prereqIt != course_index.end()) {
                    prerequisites.emplace_back(courseIt->second, prereqIt->second);
                } else {
                    cerr << "Warning: prereq pair contains unknown course: <" << course << "," << prereq << ">" << endl;
                }
            }
            pos = closePos + 1;
        }
    }

    // Create adjacency vector for prerequisites
    num_courses = static_cast<int>(course_names.size());
    prereq_adj.clear();
    prereq_adj.resize(num_courses);
    for (auto &pr : prerequisites) {
        if (pr.first >= 0 && pr.first < num_courses) {
            prereq_adj[pr.first].push_back(pr.second);
        }
    }

    computeCourseMinPeriod();

    if (!course_credits.empty() && static_cast<int>(course_credits.size()) != num_courses) {
        cerr << "Warning: credit list length (" << course_credits.size() << ") does not match course count (" << num_courses << ")." << endl;
    }

    cout << "Instance loaded: " << num_courses << " courses, " << prerequisites.size() << " prerequisites, " << num_periods << " periods." << endl;
}

