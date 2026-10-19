#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>

class Student {
private:
    std::string name;
    std::string surname;
    std::vector<int> homework;
    int exam;
    double finalGrade;

public:
    Student();
    Student(const std::string& name, const std::string& surname);
    Student(const Student& other);
    Student& operator=(const Student& other);
    ~Student();

    friend std::istream& operator>>(std::istream& in, Student& student);
    friend std::ostream& operator<<(std::ostream& out, const Student& student);

    double calculateFinalGrade(bool useMedian) const;
    void generateRandomGrades(int homeworkCount);
    void addHomeworkGrade(int grade);
    void setExam(int grade);

    std::string getName() const;
    std::string getSurname() const;
    double getFinalGrade() const;
    double getHomeworkAverage() const;
    double getHomeworkMedian() const;
    int getExam() const;
};

#endif
