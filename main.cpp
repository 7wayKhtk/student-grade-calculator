#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Student.h"
#include <algorithm>

int main() {
    std::ifstream file("students.txt");

    int inputChoice;

do {
    std::cout << "\nHow do you want to load students?\n";
    std::cout << "1 - Read students from file\n";
    std::cout << "2 - Enter students manually\n";
    std::cout << "3 - Generate random students\n";
    std::cout << "Your choice: ";

    std::cin >> inputChoice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter 1, 2, or 3.\n";
        inputChoice = 0;
    }
    else if (inputChoice < 1 || inputChoice > 3) {
        std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
    }

} while (inputChoice < 1 || inputChoice > 3);


std::vector<Student> students;


// Option 1: Read students from file
if (inputChoice == 1) {

    std::ifstream file("students.txt");

    if (!file) {
        std::cerr << "Error: Could not open students.txt" << std::endl;
        return 1;
    }
    Student student;


    while (file >> student) {
        students.push_back(student);
    }

    file.close();
}


// Option 2: Enter students manually
else if (inputChoice == 2) {

    int studentCount;

    std::cout << "How many students? ";
    std::cin >> studentCount;

    for (int i = 0; i < studentCount; ++i) {

        std::string name;
        std::string surname;
        

        std::cout << "\nStudent " << i + 1 << std::endl;

        std::cout << "Name: ";
        std::cin >> name;

        std::cout << "Surname: ";
        std::cin >> surname;

        Student student(name, surname);

        std::cout << "Enter homework grades (1-10). Enter 0 when finished:\n";

int grade;

while (true) {
    std::cout << "Homework grade: ";
    std::cin >> grade;

    if (grade == 0) {
        break;
    }

    if (grade >= 1 && grade <= 10) {
        student.addHomeworkGrade(grade);
    } else {
        std::cout << "Invalid grade. Please enter 1-10 or 0 to finish.\n";
    }
}

int exam;

std::cout << "Exam grade: ";
std::cin >> exam;

student.setExam(exam);

        students.push_back(student);
    }
}


// Option 3: Generate random students
else if (inputChoice == 3) {

    int studentCount;
    int homeworkCount;

    std::cout << "How many students? ";
    std::cin >> studentCount;

    std::cout << "How many homework grades per student? ";
    std::cin >> homeworkCount;

    for (int i = 0; i < studentCount; ++i) {

        Student student(
            "Student" + std::to_string(i + 1),
            "Surname" + std::to_string(i + 1)
        );

        student.generateRandomGrades(homeworkCount);

        students.push_back(student);
    }
}


std::cout << "\nStudents loaded: "
          << students.size()
          << std::endl;

    file.close();

    std::cout << "Students loaded: " << students.size() << std::endl;
    std::cout << std::endl;

    int sortChoice;

do {
    std::cout << "\nSort students by:\n";
    std::cout << "1 - Name\n";
    std::cout << "2 - Surname\n";
    std::cout << "3 - No sorting\n";
    std::cout << "Your choice: ";

    std::cin >> sortChoice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Please enter 1, 2, or 3.\n";
        sortChoice = 0;
    }
    else if (sortChoice < 1 || sortChoice > 3) {
        std::cout << "Invalid choice. Please enter 1, 2, or 3.\n";
    }

} while (sortChoice < 1 || sortChoice > 3);

if (sortChoice == 1) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getName() < b.getName();
        });
}
else if (sortChoice == 2) {
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.getSurname() < b.getSurname();
        });
}

    // Table header
    std::cout << std::left
              << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::setw(12) << "Average"
              << std::setw(12) << "Median"
              << std::setw(10) << "Exam"
              << std::setw(15) << "Final Avg"
              << std::setw(15) << "Final Median"
              << std::endl;

    std::cout << std::string(94, '-') << std::endl;

    // Student data
    for (const Student& s : students) {

        double finalAverage = s.calculateFinalGrade(false);
        double finalMedian = s.calculateFinalGrade(true);

        std::cout << std::left
                  << std::setw(15) << s.getName()
                  << std::setw(15) << s.getSurname()
                  << std::setw(12) << s.getHomeworkAverage()
                  << std::setw(12) << s.getHomeworkMedian()
                  << std::setw(10) << s.getExam()
                  << std::setw(15) << std::fixed << std::setprecision(2)
                  << finalAverage
                  << std::setw(15)
                  << finalMedian
                  << std::endl;
    }

    // Test Rule of Three
Student original("Test", "Student");
original.generateRandomGrades(5);

Student copy(original);      // Copy constructor

Student assigned;
assigned = original;         // Copy assignment operator

std::cout << "\nRule of Three test:" << std::endl;

std::cout << "Original: " << original.getName()
          << " " << original.getSurname() << std::endl;

std::cout << "Copy: " << copy.getName()
          << " " << copy.getSurname() << std::endl;

std::cout << "Assigned: " << assigned.getName()
          << " " << assigned.getSurname() << std::endl;

    return 0;
}