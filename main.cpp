#include "Student.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <deque>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>

using Clock = std::chrono::steady_clock;

double elapsedMs(Clock::time_point start, Clock::time_point end) {
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int readInt(const std::string& prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minValue && value <= maxValue) return value;
        std::cout << "Invalid input. Enter a number from " << minValue << " to " << maxValue << ".\n";
        if (std::cin.eof()) throw std::runtime_error("Input ended.");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void generateFile(const std::string& filename, std::size_t count) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot create file: " + filename);
    constexpr int homeworkCount = 5;
    for (std::size_t i = 1; i <= count; ++i) {
        out << "Name" << i << " Surname" << i << ' ' << homeworkCount;
        for (int j = 0; j < homeworkCount; ++j) out << ' ' << (1 + std::rand() % 10);
        out << ' ' << (1 + std::rand() % 10) << '\n';
    }
    if (!out) throw std::runtime_error("Error writing file: " + filename);
}

void generateBenchmarkFiles() {
    // Five test sizes listed in the assignment.
    const std::size_t sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    for (std::size_t count : sizes) {
        const std::string filename = "students" + std::to_string(count) + ".txt";
        std::cout << "Generating " << filename << "...\n";
        generateFile(filename, count);
    }
    std::cout << "Benchmark files generated.\n";
}

std::vector<Student> readStudents(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open input file: " + filename);

    std::vector<Student> students;
    Student student;
    while (in >> student) students.push_back(student);
    if (!in.eof()) {
        throw std::runtime_error("Invalid student data in " + filename +
            ". Expected: name surname homeworkCount grades... exam.");
    }
    return students;
}


template <typename Container>
Container readStudentsInto(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Could not open input file: " + filename);
    Container students;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        std::istringstream record(line);
        Student student;
        std::string extra;
        if (!(record >> student) || (record >> extra))
            throw std::runtime_error("Invalid record on line " + std::to_string(lineNumber));
        students.push_back(student);
    }
    if (in.bad()) throw std::runtime_error("Error reading " + filename);
    return students;
}


void sortStudents(std::vector<Student>& students, int sortChoice) {
    if (sortChoice == 1) {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            if (a.getName() != b.getName()) return a.getName() < b.getName();
            return a.getSurname() < b.getSurname();
        });
    } else {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            if (a.getSurname() != b.getSurname()) return a.getSurname() < b.getSurname();
            return a.getName() < b.getName();
        });
    }
}


void sortStudentsList(std::list<Student>& students, int sortChoice) {
    if (sortChoice == 1) {
        students.sort([](const Student& a, const Student& b) {
            if (a.getName() != b.getName())
                return a.getName() < b.getName();
            return a.getSurname() < b.getSurname();
        });
    } else {
        students.sort([](const Student& a, const Student& b) {
            if (a.getSurname() != b.getSurname())
                return a.getSurname() < b.getSurname();
            return a.getName() < b.getName();
        });
    }
}

void sortStudentsDeque(std::deque<Student>& students, int sortChoice) {
    if (sortChoice == 1) {
        std::sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                if (a.getName() != b.getName())
                    return a.getName() < b.getName();
                return a.getSurname() < b.getSurname();
            });
    } else {
        std::sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                if (a.getSurname() != b.getSurname())
                    return a.getSurname() < b.getSurname();
                return a.getName() < b.getName();
            });
    }
}


void writeGroup(const std::string& filename, const std::vector<Student>& students,
                bool passed, bool useMedian) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot create output file: " + filename);
    out << "# Format: name surname homeworkCount grades... exam finalGrade\n";
    for (const Student& student : students) {
        const double grade = student.calculateFinalGrade(useMedian);
        if ((grade >= 5.0) == passed) {
            out << student << ' ' << std::fixed << std::setprecision(2) << grade << '\n';
        }
    }
    if (!out) throw std::runtime_error("Error writing file: " + filename);
}


template <typename Container>
void writeGroupGeneric(
    const std::string& filename,
    const Container& students,
    bool passed,
    bool useMedian
) {
    std::ofstream out(filename);

    if (!out) {
        throw std::runtime_error("Cannot create output file: " + filename);
    }

    out << "# Format: name surname homeworkCount grades... exam finalGrade\n";

    for (const Student& student : students) {
        const double grade = student.calculateFinalGrade(useMedian);

        if ((grade >= 5.0) == passed) {
            out << student << ' '
                << std::fixed << std::setprecision(2)
                << grade << '\n';
        }
    }

    if (!out) {
        throw std::runtime_error("Error writing file: " + filename);
    }
}


template <typename Container>
void benchmarkContainer(
    const std::string& filename,
    const std::string& containerName,
    int sortChoice,
    bool useMedian,
    int strategy, int run, std::ostream& results
) {
    std::cout << "\n--- " << containerName
              << " | " << filename << " ---\n";

    const auto readStart = Clock::now();
    Container students = readStudentsInto<Container>(filename);
    const auto readEnd = Clock::now();
    const auto originalCount = students.size();

    const auto sortStart = Clock::now();

    if constexpr (std::is_same_v<Container, std::list<Student>>) {
        sortStudentsList(students, sortChoice);
    } else if constexpr (std::is_same_v<Container, std::deque<Student>>) {
        sortStudentsDeque(students, sortChoice);
    } else {
        sortStudents(students, sortChoice);
    }

    const auto sortEnd = Clock::now();

    std::cout << "Strategy " << strategy << '\n';
    const auto classifyStart = Clock::now();
    Container passedStudents;
    Container failedStudents;
    if (strategy == 1) {
        for (const Student& student : students) {
            if (student.calculateFinalGrade(useMedian) >= 5.0)
                passedStudents.push_back(student);
            else
                failedStudents.push_back(student);
        }
    } else {
        // Compact passed records in place, then erase the unused tail once.
        // This avoids repeatedly shifting vector/deque elements.
        auto destination = students.begin();
        for (auto current = students.begin(); current != students.end(); ++current) {
            if (current->calculateFinalGrade(useMedian) < 5.0) {
                failedStudents.push_back(*current);
            } else {
                if (destination != current) *destination = *current;
                ++destination;
            }
        }
        students.erase(destination, students.end());
        // Logical size is reduced for every container. Capacity is not a byte measurement.
    }
    const Container& passedGroup = strategy == 1 ? passedStudents : students;
    const auto passedCount = passedGroup.size();
    const auto failedCount = failedStudents.size();

    const auto classifyEnd = Clock::now();

    const auto writeStart = Clock::now();
    writeGroupGeneric(
        "passed_" + containerName + "_s" + std::to_string(strategy) + ".txt",
        passedGroup, true, useMedian
    );
    writeGroupGeneric(
        "failed_" + containerName + "_s" + std::to_string(strategy) + ".txt",
        failedStudents, false, useMedian
    );
    const auto writeEnd = Clock::now();

    const double readTime = elapsedMs(readStart, readEnd);
    const double sortTime = elapsedMs(sortStart, sortEnd);
    const double classifyTime = elapsedMs(classifyStart, classifyEnd);
    const double writeTime = elapsedMs(writeStart, writeEnd);

    results << std::quoted(filename) << ',' << containerName << ',' << strategy
            << ',' << run << ',' << sortChoice << ',' << (useMedian ? "median" : "average")
            << ',' << originalCount << ',' << passedCount << ',' << failedCount
            << ',' << std::setprecision(9) << readTime << ',' << sortTime
            << ',' << classifyTime << ',' << writeTime
            << ',' << (readTime + sortTime + classifyTime + writeTime)
            << ',' << students.size() + passedStudents.size() + failedStudents.size() << '\n';
    if (!results) throw std::runtime_error("Could not write benchmark_results.csv");

    std::cout << "Students: " << originalCount
              << "\nPassed: " << passedCount
              << "\nFailed: " << failedCount
              << std::fixed << std::setprecision(2)
              << "\nReading (ms): " << readTime
              << "\nSorting (ms): " << sortTime
              << "\nClassification (ms): " << classifyTime
              << "\nWriting (ms): " << writeTime
              << "\nTotal (ms): "
              << readTime + sortTime + classifyTime + writeTime
              << '\n';
}


int main() {
    try {
        std::srand(static_cast<unsigned int>(
            std::chrono::system_clock::now().time_since_epoch().count()));

        
std::cout << "Student Grade Calculator - Task 3 stage 2\n"
          << "1 - Process a student file (original vector mode)\n"
          << "2 - Generate benchmark files\n"
          << "3 - Compare grouping strategies and containers\n";

const int action = readInt("Your choice: ", 1, 3);

if (action == 2) {
    generateBenchmarkFiles();
    return 0;
}

if (action == 3) {
    std::string filename;
    std::cout << "Input benchmark filename: ";
    if (!std::getline(std::cin >> std::ws, filename)) throw std::runtime_error("Input ended.");

    const int sortChoice =
        readInt("Sort by: 1 - Name, 2 - Surname: ", 1, 2);

    const int gradeChoice =
        readInt("Final grade: 1 - Average, 2 - Median: ", 1, 2);

    const bool useMedian = (gradeChoice == 2);

    const int selection = readInt("Strategy: 1 - Two groups, 2 - Failed only, 3 - Both: ", 1, 3);
    const int repetitions = readInt("Repetitions (1-20): ", 1, 20);
    std::ofstream results("benchmark_results.csv");
    if (!results) throw std::runtime_error("Cannot create benchmark_results.csv");
    results << "file,container,strategy,run,sort,grade_method,students,passed,failed,read_ms,sort_ms,split_ms,write_ms,total_ms,retained_records\n";
    for (int run = 1; run <= repetitions; ++run) {
        for (int strategy = 1; strategy <= 2; ++strategy) {
            if (selection != 3 && selection != strategy) continue;
            // Rotate container order to reduce a fixed-order advantage.
            for (int offset = 0; offset < 3; ++offset) {
                const int container = (run - 1 + offset) % 3;
                if (container == 0)
                    benchmarkContainer<std::vector<Student>>(filename, "vector", sortChoice, useMedian, strategy, run, results);
                else if (container == 1)
                    benchmarkContainer<std::list<Student>>(filename, "list", sortChoice, useMedian, strategy, run, results);
                else
                    benchmarkContainer<std::deque<Student>>(filename, "deque", sortChoice, useMedian, strategy, run, results);
            }
        }
    }
    results.close();
    if (!results) throw std::runtime_error("Could not finish benchmark_results.csv");
    std::cout << "Saved benchmark_results.csv (replaces previous results).\n";

    return 0;
}


        std::string filename;
        std::cout << "Input filename (e.g. students10000.txt): ";
        if (!std::getline(std::cin >> std::ws, filename)) throw std::runtime_error("Input ended.");
        const int sortChoice = readInt("Sort by: 1 - Name, 2 - Surname: ", 1, 2);
        const int gradeChoice = readInt("Final grade: 1 - Average, 2 - Median: ", 1, 2);
        const bool useMedian = (gradeChoice == 2);

        const auto readStart = Clock::now();
        std::vector<Student> students = readStudents(filename);
        const auto readEnd = Clock::now();

        const auto sortStart = Clock::now();
        sortStudents(students, sortChoice);
        const auto sortEnd = Clock::now();

        const auto splitStart = Clock::now();
        std::size_t passedCount = 0, failedCount = 0;
        for (const Student& s : students) {
            if (s.calculateFinalGrade(useMedian) >= 5.0) ++passedCount;
            else ++failedCount;
        }
        const auto splitEnd = Clock::now();

        const auto writeStart = Clock::now();
        writeGroup("passed.txt", students, true, useMedian);
        writeGroup("failed.txt", students, false, useMedian);
        const auto writeEnd = Clock::now();

        const double readTime = elapsedMs(readStart, readEnd);
        const double sortTime = elapsedMs(sortStart, sortEnd);
        const double splitTime = elapsedMs(splitStart, splitEnd);
        const double writeTime = elapsedMs(writeStart, writeEnd);

        std::cout << "\nStudents processed: " << students.size()
                  << "\nPassed (grade >= 5.0): " << passedCount
                  << "\nFailed (grade < 5.0): " << failedCount
                  << std::fixed << std::setprecision(2)
                  << "\nExecution time (ms)\nReading: " << readTime
                  << "\nSorting: " << sortTime
                  << "\nClassification: " << splitTime
                  << "\nWriting files: " << writeTime
                  << "\nTotal: " << readTime + sortTime + splitTime + writeTime
                  << "\nCreated passed.txt and failed.txt.\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
