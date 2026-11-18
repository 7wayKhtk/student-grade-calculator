CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
PYTHON ?= python3

student_grade_calculator: main.cpp Student.cpp Student.h
	$(CXX) $(CXXFLAGS) main.cpp Student.cpp -o $@

.PHONY: test clean
test: student_grade_calculator
	$(PYTHON) scripts/check.py ./student_grade_calculator
clean:
	rm -f student_grade_calculator
