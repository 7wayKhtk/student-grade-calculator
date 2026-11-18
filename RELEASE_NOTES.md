# Student Grade Calculator v1.0

Completes Task 3 with two student-grouping strategies for vector, list and deque, selectable from the menu. Includes sorting by name/surname, average/median final grades, passed/failed output, repeated benchmarks and CSV export.

The selected list/strategy-2 optimization uses std::find_if with list::splice. Generic algorithm alternatives use copy_if, remove_copy_if and remove_if. RESULTS.md documents all five required sizes, measured memory use and paired before/after results. Raw measurements and reproduction scripts are included.

Build with Make, CMake, or g++ using C++17. Make and CMake/CTest checks passed, including 192 valid cases and malformed-input checks. Python is optional for running the calculator and required only for the supplied tests/benchmark scripts.

Linux memory measurements use /proc and getrusage. Other platforms report -1 for unavailable memory metrics. Timings are environment-specific. No prebuilt binaries or generated large datasets are distributed.
