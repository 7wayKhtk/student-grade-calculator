# Student Grade Calculator v0.25

## Overview

This release extends v0.2 by comparing the performance of three C++ containers:

- `std::vector`
- `std::list`
- `std::deque`

The program reads student records, sorts them by name or surname, calculates final grades, classifies students as passed or failed, writes the results to separate files, and measures execution time.

## Features

- Student class implemented in separate `Student.h` and `Student.cpp` files.
- Exception handling for file and invalid student-data errors.
- Generation of benchmark datasets containing 10,000, 100,000, 1,000,000, and 10,000,000 student records.
- Additional 1,000-student dataset for benchmarking.
- Sorting by name or surname.
- Final grade calculated using homework average or median:

  `Final grade = 0.4 × homework result + 0.6 × exam grade`

- Students with a final grade greater than or equal to 5.0 are classified as passed.
- Students with a final grade below 5.0 are classified as failed.
- Separate output files for passed and failed students.
- Execution-time measurements for reading, sorting, classification, and writing.
- Performance comparison of `std::vector`, `std::list`, and `std::deque`.

## Compilation and execution

Compile with GCC:

```powershell
g++ -std=c++17 -Wall -Wextra -O2 main.cpp Student.cpp -o StudentGradeCalculator.exe
```

Run the program:

```powershell
.\StudentGradeCalculator.exe
```

Choose option 3 to compare the containers. Use the same dataset, sorting option, and grade calculation method for each comparison.

## Benchmark environment

- Operating system: Windows
- Compiler: GCC / MinGW
- C++ standard: C++17
- Optimization: `-O2`
- Sorting: By name
- Final grade calculation: Homework average

All execution times are in milliseconds. These measurements are specific to the machine and environment used for testing.

## Benchmark results: 1,000 students

| Container | Reading | Sorting | Classification | Writing | Total |
|---|---:|---:|---:|---:|---:|
| `std::vector` | 3.16 | 1.19 | 0.02 | 6.00 | 10.38 |
| `std::list` | 2.55 | 0.32 | 0.03 | 7.31 | 10.22 |
| `std::deque` | 1.71 | 1.57 | 0.03 | 7.02 | 10.33 |

## Benchmark results: 10,000 students

| Container | Reading | Sorting | Classification | Writing | Total |
|---|---:|---:|---:|---:|---:|
| `std::vector` | 165.81 | 13.90 | 0.18 | 20.64 | 200.53 |
| `std::list` | 19.71 | 4.91 | 1.79 | 23.12 | 49.53 |
| `std::deque` | 18.73 | 18.66 | 0.26 | 22.17 | 59.82 |

## Benchmark results: 100,000 students

| Container | Reading | Sorting | Classification | Writing | Total |
|---|---:|---:|---:|---:|---:|
| `std::vector` | 169.89 | 160.70 | 2.02 | 223.22 | 555.83 |
| `std::list` | 159.41 | 42.68 | 3.15 | 231.27 | 436.52 |
| `std::deque` | 142.89 | 179.32 | 2.51 | 186.20 | 510.92 |

## Benchmark results: 1,000,000 students

| Container | Reading | Sorting | Classification | Writing | Total |
|---|---:|---:|---:|---:|---:|
| `std::vector` | 1499.66 | 2117.96 | 21.40 | 1832.16 | 5471.19 |
| `std::list` | 1522.77 | 561.91 | 35.37 | 1753.24 | 3873.31 |
| `std::deque` | 1425.28 | 2603.85 | 27.23 | 1713.05 | 5769.40 |

## Benchmark results: 10,000,000 students

| Container | Reading | Sorting | Classification | Writing | Total |
|---|---:|---:|---:|---:|---:|
| `std::vector` | 25159.69 | 29779.56 | 208.85 | 20297.72 | 75445.83 |
| `std::list` | 15805.20 | 6756.44 | 341.32 | 24474.14 | 47377.11 |
| `std::deque` | 15122.58 | 30053.60 | 264.46 | 23060.03 | 68500.67 |

## Classification verification

All three containers produced the same passed and failed counts for each dataset.

| Students | Passed | Failed |
|---:|---:|---:|
| 1,000 | 557 | 443 |
| 10,000 | 5,799 | 4,201 |
| 100,000 | 58,686 | 41,314 |
| 1,000,000 | 585,900 | 414,100 |
| 10,000,000 | 5,864,969 | 4,135,031 |

The passed and failed counts add up to the total number of students in every dataset.

## Performance analysis

The results show that container performance depends on dataset size and the operation being measured.

- `std::vector` provides contiguous storage and supports random-access sorting with `std::sort`.
- `std::list` uses its own member `sort()` function, which can perform well for large datasets when sorting is a significant part of the workload.
- `std::deque` supports random-access iterators and can use `std::sort`, but its measured sorting time was higher than the list implementation in the larger tests.
- Reading and writing times also contribute substantially to total execution time.

In these measurements, `std::list` had the lowest total time for the 10,000, 100,000, 1,000,000, and 10,000,000 student tests. The 1,000-student test showed very similar total times for all three containers.

These conclusions apply to the measured environment and should not be treated as universal performance rankings.

## Output files

The program writes passed and failed students into separate output files for each container, using the container name in the filename.

- `passed_vector.txt` and `failed_vector.txt`
- `passed_list.txt` and `failed_list.txt`
- `passed_deque.txt` and `failed_deque.txt`

## Exception handling

The program reports errors when an input file cannot be opened, student records contain invalid data, or output files cannot be created or written.

## Conclusion

Version v0.25 extends the Student Grade Calculator with implementations using `std::vector`, `std::list`, and `std::deque`. The benchmark results demonstrate that container choice can affect reading, sorting, classification, writing, and total execution time. The most suitable container depends on the dataset and the workload.
