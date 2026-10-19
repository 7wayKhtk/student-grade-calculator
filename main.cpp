#include "Student.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

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

int main() {
    try {
        std::srand(static_cast<unsigned int>(
            std::chrono::system_clock::now().time_since_epoch().count()));

        std::cout << "Student Grade Calculator v0.2\n"
                  << "1 - Process a student file\n"
                  << "2 - Generate benchmark files\n";
        const int action = readInt("Your choice: ", 1, 2);
        if (action == 2) {
            generateBenchmarkFiles();
            return 0;
        }

        std::string filename;
        std::cout << "Input filename (e.g. students10000.txt): ";
        std::cin >> filename;
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
