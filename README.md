# Student Grade Calculator — Task 3 stage 3

Approximate 88% planning milestone, based on the existing 75% checkpoint. This is a progress version, not the final v1.0 release.

## Progress

- v0.1: initial calculator (original tag preserved).
- v0.2: file-processing/benchmark groundwork (original tag preserved).
- v0.25: container comparisons.
- Stage 1: Strategy 1 and build files.
- Stage 2: Strategy 2, repeated comparisons and CSV.
- Stage 3: isolated benchmark commands, real Linux memory measurements, reproducible full-size datasets and the complete six-way baseline survey through 10M students.

Final algorithm optimization, before/after comparisons and release/submission remain for the next stage. STAGE2_RESULTS.md remains historical; this stage uses the unified CSV format described below.

## Build and run

Unix: `make`, then `./student_grade_calculator`.

Windows / VS Code, from the existing task3-repository folder:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp Student.cpp -o student_grade_calculator.exe
.\student_grade_calculator.exe
```

CMake alternative:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Executable: build/student_grade_calculator on Unix; build/student_grade_calculator.exe for MinGW or build/Release/student_grade_calculator.exe for Visual Studio generators. Requires CMake and a configured C++17 compiler.

## Menu

1. Process an input file with vector.
2. Generate 1K, 10K, 100K, 1M and 10M student files (substantial disk space).
3. Compare vector, list and deque.

Processing asks for filename, sort by name/surname, average/median and strategy 1/2/both. Comparison also asks for 1–20 repetitions. This stage runs original loops only; algorithm selection is intentionally reserved for the final stage.

Strategy 1 retains the source and creates two same-type groups. Strategy 2 copies failures to a new container, compacts passed records in the source and erases the unused tail once. Erase updates the container size; it need not shrink vector/deque capacity.

Example input (one record per line):

```text
Youssef ElQouqi 5 5 8 9 10 7 9
```

Fields: name, surname, homework count, homework grades, exam. Names are single tokens; count 0–1000; grades 1–10. Final grade = 0.4 × homework average/median + 0.6 × exam. Grade >= 5 passes. Zero homework contributes zero. Blank lines are skipped; malformed/truncated/extra fields are rejected. A small students.txt is included.

Outputs include passed_list_s2_baseline.txt and failed_list_s2_baseline.txt (corresponding names for other containers/strategies). Reports include final grades and are not input files. Repeated runs overwrite matching report files. The interactive CSV is benchmark_results.csv and is replaced each session; rename it if you want to retain it.

## Isolated benchmarking and verification

Python 3 is optional for the calculator and required for these helper scripts. Windows commands:

```powershell
python scripts/check.py .\student_grade_calculator.exe
python scripts/benchmark.py --exe .\student_grade_calculator.exe --output results/my-baseline.csv --sizes 1000 10000 --repetitions 3
```

Omit --sizes to measure all five required sizes; use --repetitions 1 to reproduce the survey. The runner creates temporary datasets and deletes them after use. Run from a location with sufficient space. It rotates case order across repetitions and launches a new process for each measurement.

Single-case commands:

```powershell
.\student_grade_calculator.exe --generate 1000 students1000.txt
.\student_grade_calculator.exe --run students1000.txt list 2 1 1 baseline 1 one.csv
```

--run arguments: input, container, strategy, sort, grade, baseline, run number, CSV. Run where its output filenames can safely be replaced.

Read/sort/split/write timings are separate; total is their sum. Menu time, CSV logging, memory probes, input generation and container destruction are excluded. Files are closed and write errors checked, but physical disk synchronization is not forced.

Linux RSS comes from /proc/self/status and peak RSS from getrusage, in KiB. On Windows/unsupported systems these columns contain -1, meaning unavailable. Interactive peak RSS is cumulative; use fresh-process measurements for comparisons. These Linux readings do not describe your Windows computer.

The tests cover 96 valid combinations: empty/all-pass/all-fail/mixed records, grade-5 boundary, zero homework, average/median, both sorting choices, both strategies and all containers. Invalid/missing inputs and EOF are also checked.

## Recorded results

### Stage 3 baseline measurements

Recorded on 9 November 2026 during the previous final-stage investigation and reused here because this checkpoint uses that same baseline implementation. These are measured results, not estimates, and were not rerun for this packaging step.

Linux x86_64, GCC 14.2.0, C++17, -O2; see results/baseline.json. Deterministic five-homework records; sort by name; use average. One fresh process per case; caches not cleared. The single-run survey identifies a candidate to investigate, not a universal winner.

| Students | Container | Strategy | Read ms | Sort ms | Split ms | Write ms | Total ms | RSS after split MiB |
|---:|---|---:|---:|---:|---:|---:|---:|---:|
| 1000 | vector | 1 | 0.408 | 0.438 | 0.094 | 0.347 | 1.287 | 2.20 |
| 1000 | vector | 2 | 0.405 | 0.401 | 0.066 | 0.331 | 1.203 | 2.14 |
| 1000 | list | 1 | 0.422 | 0.129 | 0.050 | 0.265 | 0.866 | 2.12 |
| 1000 | list | 2 | 0.354 | 0.129 | 0.043 | 0.253 | 0.779 | 2.05 |
| 1000 | deque | 1 | 0.302 | 0.525 | 0.048 | 0.256 | 1.130 | 2.09 |
| 1000 | deque | 2 | 0.317 | 0.390 | 0.039 | 0.513 | 1.259 | 2.04 |
| 10000 | vector | 1 | 4.128 | 6.163 | 0.897 | 2.351 | 13.540 | 5.28 |
| 10000 | vector | 2 | 4.163 | 5.877 | 0.916 | 3.356 | 14.312 | 4.63 |
| 10000 | list | 1 | 10.756 | 2.723 | 1.623 | 5.978 | 21.080 | 4.87 |
| 10000 | list | 2 | 3.132 | 2.071 | 0.463 | 2.489 | 8.155 | 4.25 |
| 10000 | deque | 1 | 3.140 | 6.841 | 0.536 | 2.762 | 13.278 | 4.54 |
| 10000 | deque | 2 | 2.967 | 6.596 | 0.400 | 2.627 | 12.591 | 3.99 |
| 100000 | vector | 1 | 35.033 | 79.044 | 10.743 | 25.459 | 150.280 | 34.59 |
| 100000 | vector | 2 | 35.491 | 78.065 | 6.216 | 25.077 | 144.849 | 27.39 |
| 100000 | list | 1 | 32.408 | 23.189 | 8.251 | 30.250 | 94.098 | 32.33 |
| 100000 | list | 2 | 31.461 | 22.724 | 4.743 | 24.010 | 82.939 | 26.22 |
| 100000 | deque | 1 | 33.111 | 92.860 | 4.752 | 29.587 | 160.310 | 28.93 |
| 100000 | deque | 2 | 30.540 | 81.599 | 4.293 | 25.317 | 141.748 | 23.50 |
| 1000000 | vector | 1 | 368.914 | 1131.734 | 159.268 | 240.219 | 1900.135 | 273.38 |
| 1000000 | vector | 2 | 335.729 | 1089.213 | 80.443 | 232.284 | 1737.669 | 243.71 |
| 1000000 | list | 1 | 298.801 | 255.211 | 66.784 | 260.252 | 881.048 | 306.99 |
| 1000000 | list | 2 | 342.908 | 267.733 | 53.783 | 256.027 | 920.451 | 245.95 |
| 1000000 | deque | 1 | 294.750 | 1033.606 | 58.708 | 325.958 | 1713.022 | 272.69 |
| 1000000 | deque | 2 | 306.171 | 1109.358 | 50.206 | 252.714 | 1718.448 | 218.52 |
| 10000000 | vector | 1 | 11564.660 | 13653.767 | 2196.356 | 2684.767 | 30099.550 | 2631.98 |
| 10000000 | vector | 2 | 3832.982 | 13304.537 | 811.536 | 2463.554 | 20412.609 | 2149.92 |
| 10000000 | list | 1 | 3052.914 | 3147.348 | 771.396 | 2564.739 | 9536.397 | 3053.57 |
| 10000000 | list | 2 | 3134.905 | 3092.422 | 554.860 | 2352.544 | 9134.731 | 2443.21 |
| 10000000 | deque | 1 | 3017.461 | 13896.854 | 522.515 | 2972.296 | 20409.127 | 2710.29 |
| 10000000 | deque | 2 | 3013.849 | 13793.138 | 532.129 | 3084.985 | 20424.102 | 2168.59 |

At 10M records, list/strategy 2 had the lowest surveyed total (9.135 seconds). It is the candidate for the next stage, when standard algorithms and repeated before/after measurements will be added.

RSS after splitting is resident process memory, not a sum of Student object sizes. Strategy 1 retains 2N live records; Strategy 2 retains N. Vector/deque capacity, list nodes, allocator retention and dynamic strings/homework affect allocated bytes. Erase reduces logical size without guaranteeing capacity release.

For 10M records, strategy 1 / strategy 2 resident memory was approximately vector 2632 / 2150 MiB, list 3054 / 2443 MiB, deque 2710 / 2169 MiB. This supports a memory benefit, not an exact twofold saving. Linux process peak RSS is also in the raw CSV; interactive runs accumulate peaks, so the isolated CLI was used. The OS counters may differ slightly due to sampling/accounting.

Still pending: standard-algorithm refactor, repeated before/after results, final v1.0 branch/tag/release and Moodle submission.
