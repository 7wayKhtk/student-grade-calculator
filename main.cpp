#include "Student.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#ifdef __linux__
#include <sys/resource.h>
#endif
using namespace std;
using Clock = chrono::steady_clock;

const char* csvHeader = "file,container,strategy,implementation,run,sort,grade_method,students,passed,failed,read_ms,sort_ms,split_ms,write_ms,total_ms,retained_records,rss_after_split_kib,peak_rss_kib\n";

double elapsedMs(Clock::time_point start, Clock::time_point end) {
    return chrono::duration<double, milli>(end - start).count();
}

long residentKiB() {
#ifdef __linux__
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            istringstream value(line.substr(6));
            long kib;
            if (value >> kib) return kib;
        }
    }
#endif
    return -1; // Unsupported: never report a missing measurement as zero.
}
long peakKiB() {
#ifdef __linux__
    rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0) return usage.ru_maxrss;
#endif
    return -1;
}

int number(const string& text, int low, int high) {
    istringstream input(text);
    int value;
    char extra;
    if (!(input >> value) || (input >> extra) || value < low || value > high)
        throw runtime_error("Expected an integer from " + to_string(low) + " to " + to_string(high));
    return value;
}
int readInt(const string& prompt, int low, int high) {
    while (true) {
        cout << prompt;
        string text;
        if (!getline(cin, text)) throw runtime_error("Input ended.");
        try { return number(text, low, high); }
        catch (const runtime_error& error) { cout << error.what() << '\n'; }
    }
}
string readFilename() {
    cout << "Input filename: ";
    string filename;
    if (!getline(cin, filename) || filename.empty()) throw runtime_error("Input filename required.");
    return filename;
}
void generateFile(const string& filename, size_t count) {
    ofstream out(filename);
    if (!out) throw runtime_error("Cannot create " + filename);
    // Fixed arithmetic pattern: identical datasets across platforms and runs.
    for (size_t i = 1; i <= count; ++i) {
        out << "Name" << i << " Surname" << i << " 5";
        for (size_t j = 0; j < 6; ++j) out << ' ' << (i + j) % 10 + 1;
        out << '\n';
    }
    out.close();
    if (!out) throw runtime_error("Error writing " + filename);
}

template <typename Container>
Container readStudents(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("Cannot open " + filename);
    Container students;
    string line;
    size_t lineNumber = 0;
    while (getline(in, line)) {
        ++lineNumber;
        if (line.find_first_not_of(" \t\r") == string::npos) continue;
        istringstream record(line);
        Student student;
        string extra;
        if (!(record >> student) || (record >> extra))
            throw runtime_error("Invalid student record on line " + to_string(lineNumber));
        students.push_back(student);
    }
    if (in.bad()) throw runtime_error("Error reading " + filename);
    return students;
}

template <typename Container>
void sortStudents(Container& students, int choice) {
    auto compare = [choice](const Student& a, const Student& b) {
        if (choice == 1) {
            if (a.getName() != b.getName()) return a.getName() < b.getName();
            return a.getSurname() < b.getSurname();
        }
        if (a.getSurname() != b.getSurname()) return a.getSurname() < b.getSurname();
        return a.getName() < b.getName();
    };
    if constexpr (is_same_v<Container, list<Student>>) students.sort(compare);
    else sort(students.begin(), students.end(), compare);
}

template <typename Container>
void splitStudents(Container& students, Container& passed, Container& failed,
                   int strategy, bool median, const string& implementation) {
    if (implementation != "baseline") throw runtime_error("Unknown implementation");
    if (strategy == 1) {
        for (const Student& student : students) {
            if (student.calculateFinalGrade(median) >= 5.0) passed.push_back(student);
            else failed.push_back(student);
        }
    } else {
        auto destination = students.begin();
        for (auto current = students.begin(); current != students.end(); ++current) {
            if (current->calculateFinalGrade(median) < 5.0) failed.push_back(*current);
            else {
                if (destination != current) *destination = *current;
                ++destination;
            }
        }
        students.erase(destination, students.end());
    }
}

template <typename Container>
void writeGroup(const string& filename, const Container& students, bool median) {
    ofstream out(filename);
    if (!out) throw runtime_error("Cannot create " + filename);
    out << "# name surname homeworkCount grades... exam finalGrade\n";
    for (const Student& student : students)
        out << student << ' ' << fixed << setprecision(2) << student.calculateFinalGrade(median) << '\n';
    out.close();
    if (!out) throw runtime_error("Error writing " + filename);
}

template <typename Container>
void benchmark(const string& filename, const string& container, int strategy, int sortChoice,
               bool median, const string& implementation, int run, ostream& results) {
    const auto t0 = Clock::now();
    Container students = readStudents<Container>(filename);
    const auto t1 = Clock::now();
    const auto count = students.size();
    sortStudents(students, sortChoice);
    const auto t2 = Clock::now();
    Container passed, failed;
    splitStudents(students, passed, failed, strategy, median, implementation);
    const auto t3 = Clock::now();
    const Container& passedGroup = strategy == 1 ? passed : students;
    const long rss = residentKiB(); // Excluded from phase timings.
    const auto w0 = Clock::now();
    const string suffix = container + "_s" + to_string(strategy) + "_" + implementation + ".txt";
    writeGroup("passed_" + suffix, passedGroup, median);
    writeGroup("failed_" + suffix, failed, median);
    const auto w1 = Clock::now();
    const double read = elapsedMs(t0, t1), sorting = elapsedMs(t1, t2);
    const double split = elapsedMs(t2, t3), writing = elapsedMs(w0, w1);
    results << quoted(filename) << ',' << container << ',' << strategy << ',' << implementation
            << ',' << run << ',' << sortChoice << ',' << (median ? "median" : "average")
            << ',' << count << ',' << passedGroup.size() << ',' << failed.size()
            << ',' << setprecision(10) << read << ',' << sorting << ',' << split << ',' << writing
            << ',' << read + sorting + split + writing
            << ',' << students.size() + passed.size() + failed.size() << ',' << rss << ',' << peakKiB() << '\n';
    if (!results) throw runtime_error("Cannot write result CSV");
    cout << container << " strategy " << strategy << ' ' << implementation << ": "
         << passedGroup.size() << " passed, " << failed.size() << " failed; total "
         << read + sorting + split + writing << " ms\n";
}
void dispatch(const string& file, const string& container, int strategy, int sorting,
              bool median, const string& implementation, int run, ostream& results) {
    if (container == "vector") benchmark<vector<Student>>(file, container, strategy, sorting, median, implementation, run, results);
    else if (container == "list") benchmark<list<Student>>(file, container, strategy, sorting, median, implementation, run, results);
    else if (container == "deque") benchmark<deque<Student>>(file, container, strategy, sorting, median, implementation, run, results);
    else throw runtime_error("Container must be vector, list or deque");
}

int main(int argc, char** argv) {
    try {
        if (argc == 4 && string(argv[1]) == "--generate") {
            generateFile(argv[3], number(argv[2], 1, 10000000));
            return 0;
        }
        // One process per measurement makes peak RSS comparable across cases.
        if (argc == 10 && string(argv[1]) == "--run") {
            const string file = argv[2], output = argv[9];
            if (file == output) throw runtime_error("Input and CSV filenames must differ");
            ofstream results(output);
            if (!results) throw runtime_error("Cannot create result CSV");
            results << csvHeader;
            dispatch(file, argv[3], number(argv[4], 1, 2), number(argv[5], 1, 2),
                     number(argv[6], 1, 2) == 2, argv[7], number(argv[8], 1, 20), results);
            results.close();
            if (!results) throw runtime_error("Cannot finish result CSV");
            return 0;
        }
        if (argc != 1) throw runtime_error("Usage: --generate COUNT FILE or --run FILE CONTAINER STRATEGY SORT GRADE baseline RUN CSV");
        cout << "Student Grade Calculator v1.0\n1 - Process a file\n2 - Generate benchmark files\n3 - Compare containers and strategies\n";
        const int action = readInt("Choice: ", 1, 3);
        if (action == 2) {
            for (size_t n : {1000, 10000, 100000, 1000000, 10000000}) {
                cout << "Generating " << n << " records...\n";
                generateFile("students" + to_string(n) + ".txt", n);
            }
            return 0;
        }
        const string file = readFilename();
        const int sorting = readInt("Sort: 1 - Name, 2 - Surname: ", 1, 2);
        const bool median = readInt("Grade: 1 - Average, 2 - Median: ", 1, 2) == 2;
        const int strategy = readInt("Strategy: 1 - Two new groups, 2 - Failed only, 3 - Both: ", 1, 3);
        const int reps = action == 3 ? readInt("Repetitions (1-20): ", 1, 20) : 1;
        const string implementation = "baseline";
        ofstream results("benchmark_results.csv");
        if (!results) throw runtime_error("Cannot create benchmark_results.csv");
        results << csvHeader;
        const vector<string> containers = action == 3 ? vector<string>{"vector", "list", "deque"} : vector<string>{"vector"};
        for (int run = 1; run <= reps; ++run)
            for (int s = 1; s <= 2; ++s)
                if (strategy == 3 || strategy == s)
                    for (size_t offset = 0; offset < containers.size(); ++offset)
                        dispatch(file, containers[(run - 1 + offset) % containers.size()], s, sorting, median, implementation, run, results);
        results.close();
        if (!results) throw runtime_error("Cannot finish result CSV");
        cout << "Saved benchmark_results.csv.\n";
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
