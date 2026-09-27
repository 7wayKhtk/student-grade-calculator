#include "Student.h"
#include <algorithm>
#include <cstdlib>

// Default constructor
Student::Student()
    : name(""), surname(""), homework(), exam(0), finalGrade(0.0) {
}

// Constructor with name and surname
Student::Student(const std::string& name, const std::string& surname)
    : name(name), surname(surname), homework(), exam(0), finalGrade(0.0) {
}

// Copy constructor
Student::Student(const Student& other)
    : name(other.name),
      surname(other.surname),
      homework(other.homework),
      exam(other.exam),
      finalGrade(other.finalGrade) {
}

// Copy assignment operator
Student& Student::operator=(const Student& other) {
    if (this != &other) {
        name = other.name;
        surname = other.surname;
        homework = other.homework;
        exam = other.exam;
        finalGrade = other.finalGrade;
    }

    return *this;
}

// Destructor
Student::~Student() {
}

// Calculate final grade using homework average or median
double Student::calculateFinalGrade(bool useMedian) const {
    if (homework.empty()) {
        return 0.6 * exam;
    }

    double homeworkResult;

    if (useMedian) {
        std::vector<int> sortedHomework = homework;

        std::sort(sortedHomework.begin(), sortedHomework.end());

        size_t n = sortedHomework.size();

        if (n % 2 == 1) {
            homeworkResult = sortedHomework[n / 2];
        } else {
            homeworkResult =
                (sortedHomework[n / 2 - 1] + sortedHomework[n / 2]) / 2.0;
        }
    } else {
        double sum = 0;

        for (int grade : homework) {
            sum += grade;
        }

        homeworkResult = sum / homework.size();
    }

    return 0.4 * homeworkResult + 0.6 * exam;
}
// Input operator
std::istream& operator>>(std::istream& in, Student& student) {
    in >> student.name >> student.surname;

    int homeworkCount;
    in >> homeworkCount;

    student.homework.clear();

    for (int i = 0; i < homeworkCount; ++i) {
        int grade;
        in >> grade;
        student.homework.push_back(grade);
    }

    in >> student.exam;
;

    return in;
}

// Output operator
std::ostream& operator<<(std::ostream& out, const Student& student) {
    out << student.name << " "
        << student.surname << " "
        << student.finalGrade;

    return out;
}

// Getters
std::string Student::getName() const {
    return name;
}

std::string Student::getSurname() const {
    return surname;
}

double Student::getFinalGrade() const {
    return finalGrade;
}

void Student::generateRandomGrades(int homeworkCount) {
    homework.clear();

    for (int i = 0; i < homeworkCount; ++i) {
        homework.push_back(1 + std::rand() % 10);
    }

    exam = 1 + std::rand() % 10;
}

double Student::getHomeworkAverage() const {
    if (homework.empty()) {
        return 0.0;
    }

    double sum = 0;

    for (int grade : homework) {
        sum += grade;
    }

    return sum / homework.size();
}

double Student::getHomeworkMedian() const {
    if (homework.empty()) {
        return 0.0;
    }

    std::vector<int> sortedHomework = homework;

    std::sort(sortedHomework.begin(), sortedHomework.end());

    size_t n = sortedHomework.size();

    if (n % 2 == 1) {
        return sortedHomework[n / 2];
    }

    return (sortedHomework[n / 2 - 1] + sortedHomework[n / 2]) / 2.0;
}

int Student::getExam() const {
    return exam;
}

void Student::addHomeworkGrade(int grade) {
    homework.push_back(grade);
}

void Student::setExam(int grade) {
    exam = grade;
}