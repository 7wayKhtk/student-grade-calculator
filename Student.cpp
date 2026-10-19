#include "Student.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>

Student::Student()
    : name(""), surname(""), homework(), exam(0), finalGrade(0.0) {}

Student::Student(const std::string& name, const std::string& surname)
    : name(name), surname(surname), homework(), exam(0), finalGrade(0.0) {}

Student::Student(const Student& other)
    : name(other.name), surname(other.surname), homework(other.homework),
      exam(other.exam), finalGrade(other.finalGrade) {}

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

Student::~Student() = default;

double Student::calculateFinalGrade(bool useMedian) const {
    const double homeworkResult = useMedian ? getHomeworkMedian() : getHomeworkAverage();
    return 0.4 * homeworkResult + 0.6 * exam;
}

// Record format: name surname homeworkCount grade1 ... gradeN exam
std::istream& operator>>(std::istream& in, Student& student) {
    std::string newName, newSurname;
    int homeworkCount = 0;
    if (!(in >> newName >> newSurname >> homeworkCount)) return in;
    if (homeworkCount < 0 || homeworkCount > 1000) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::vector<int> newHomework;
    newHomework.reserve(static_cast<std::size_t>(homeworkCount));
    for (int i = 0; i < homeworkCount; ++i) {
        int grade;
        if (!(in >> grade) || grade < 1 || grade > 10) {
            in.setstate(std::ios::failbit);
            return in;
        }
        newHomework.push_back(grade);
    }

    int newExam;
    if (!(in >> newExam) || newExam < 1 || newExam > 10) {
        in.setstate(std::ios::failbit);
        return in;
    }

    student.name = newName;
    student.surname = newSurname;
    student.homework = newHomework;
    student.exam = newExam;
    student.finalGrade = 0.0;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Student& student) {
    out << student.name << ' ' << student.surname << ' ' << student.homework.size();
    for (int grade : student.homework) out << ' ' << grade;
    out << ' ' << student.exam;
    return out;
}

std::string Student::getName() const { return name; }
std::string Student::getSurname() const { return surname; }
double Student::getFinalGrade() const { return finalGrade; }
int Student::getExam() const { return exam; }

void Student::generateRandomGrades(int homeworkCount) {
    if (homeworkCount < 0 || homeworkCount > 1000)
        throw std::invalid_argument("Homework count must be between 0 and 1000.");
    homework.clear();
    for (int i = 0; i < homeworkCount; ++i) homework.push_back(1 + std::rand() % 10);
    exam = 1 + std::rand() % 10;
}

double Student::getHomeworkAverage() const {
    if (homework.empty()) return 0.0;
    double sum = 0.0;
    for (int grade : homework) sum += grade;
    return sum / homework.size();
}

double Student::getHomeworkMedian() const {
    if (homework.empty()) return 0.0;
    std::vector<int> sorted = homework;
    std::sort(sorted.begin(), sorted.end());
    const std::size_t n = sorted.size();
    if (n % 2 == 1) return sorted[n / 2];
    return (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
}

void Student::addHomeworkGrade(int grade) {
    if (grade < 1 || grade > 10)
        throw std::invalid_argument("Homework grades must be between 1 and 10.");
    homework.push_back(grade);
}

void Student::setExam(int grade) {
    if (grade < 1 || grade > 10)
        throw std::invalid_argument("Exam grade must be between 1 and 10.");
    exam = grade;
}
