cmake_minimum_required(VERSION 3.16)
project(StudentGradeCalculator LANGUAGES CXX)
add_executable(student_grade_calculator main.cpp Student.cpp)
target_compile_features(student_grade_calculator PRIVATE cxx_std_17)
