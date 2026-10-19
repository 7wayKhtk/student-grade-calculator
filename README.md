# Student Grade Calculator v0.2

- Reads student records from a file.
- Generates benchmark files of 1,000, 10,000, 100,000, 1,000,000 and 10,000,000 students.
- Sorts by name or surname.
- Calculates final grade with homework average or median: 0.4 * homework + 0.6 * exam.
- Writes passed students (final grade >= 5.0) to `passed.txt` and failed students (final grade < 5.0) to `failed.txt`.
- Measures reading, sorting, classification and writing time.
- Uses exceptions for file and input-data errors.

Input record format, one student per line:
`Name Surname homeworkCount homeworkGrade1 ... homeworkGradeN examGrade`

Example:
`Youssef ElQouqi 5 5 8 9 10 7 9`

Compile in PowerShell with g++:
```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp Student.cpp -o student_grade_calculator.exe
.\student_grade_calculator.exe
```

Generating the 10-million-student file requires substantial disk space and time.
