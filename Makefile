CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

student_grade_calculator: main.cpp Student.cpp Student.h
	$(CXX) $(CXXFLAGS) main.cpp Student.cpp -o $@

.PHONY: clean
clean:
	rm -f student_grade_calculator
