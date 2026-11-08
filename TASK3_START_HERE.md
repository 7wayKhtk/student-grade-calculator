# Task 3 — first progress stage

Based on the actual v0.25 tag, not main. This is an approximate 35% milestone, not the final v1.0 release.

Completed: Strategy 1 copies each student to the appropriate passed/failed container of the same type, retains the original, times grouping separately and writes both groups. Menu option 3 runs vector, list and deque on the same input. Existing option 1 and file generation remain available.

Remaining: Strategy 2 and its menu selection; repeated measurements on all required file sizes; memory investigation; choosing the fastest combination; algorithm refactoring and before/after comparison; final README results and v1.0 release.

## VS Code on Windows
Open this whole project folder. Do not use the earlier StudentGroups.h file. In the VS Code terminal with your existing g++ installation:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp Student.cpp -o student_grade_calculator.exe
.\student_grade_calculator.exe
```
Choose 3, supply an existing student input filename, then choose sorting and average/median. Output files are passed_vector.txt, failed_vector.txt and corresponding list/deque files. These are overwritten on subsequent runs. Input records: Name Surname homeworkCount grades... exam.

CMake alternative (requires CMake and an installed C++ compiler):
```
cmake -S . -B build
cmake --build build --config Release
```
The executable is under build (or build/Release for Visual Studio generators). Unix: run make, then ./student_grade_calculator.

## Progress commits
This branch starts at v0.25. Commit 1 implements Strategy 1; commit 2 adds build files and this guide. Local commits are not GitHub pushes. Review this stage before pushing your branch. Continue future work as new meaningful commits; do not create the final v1.0 release yet.

## Teacher attachments
cmaketest.zip contains a CMake example and C/C++ file-reading timing examples. Its CMake file uses a source glob and include directory. This project lists its two source files explicitly. The MSI attachment is a Windows CMake installer, not assignment source code; it was not executed.
