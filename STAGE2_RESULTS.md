# Stage 2 preliminary measurements

Environment: Linux x86_64, g++ -O2, C++17. Synthetic deterministic five-homework records. Three runs per combination, name sort, average grades. File writing included in total. OS file caches were not cleared. Results are local observations, not universal rankings.

| Records | Container | Strategy | Mean split ms | Mean total ms |
|---:|---|---:|---:|---:|
| 1000 | vector | 1 | 0.620 | 3.412 |
| 1000 | vector | 2 | 0.137 | 3.300 |
| 1000 | list | 1 | 0.101 | 1.777 |
| 1000 | list | 2 | 0.060 | 1.717 |
| 1000 | deque | 1 | 0.055 | 2.454 |
| 1000 | deque | 2 | 0.092 | 2.562 |
| 10000 | vector | 1 | 5.719 | 46.190 |
| 10000 | vector | 2 | 1.838 | 43.117 |
| 10000 | list | 1 | 1.682 | 22.573 |
| 10000 | list | 2 | 0.818 | 25.149 |
| 10000 | deque | 1 | 0.662 | 44.066 |
| 10000 | deque | 2 | 1.271 | 48.037 |
| 100000 | vector | 1 | 72.001 | 596.982 |
| 100000 | vector | 2 | 17.862 | 515.972 |
| 100000 | list | 1 | 14.852 | 241.485 |
| 100000 | list | 2 | 11.997 | 252.545 |
| 100000 | deque | 1 | 9.467 | 501.358 |
| 100000 | deque | 2 | 8.062 | 482.693 |

Strategy 1 retains 2N live student records after splitting; Strategy 2 retains N. These counts are not allocated bytes or peak RSS: vector capacity, list nodes, deque blocks and string/homework storage affect memory. Vector/deque capacity may remain after erasing.

Remaining: 1M/10M datasets, memory measurements, choosing and refactoring the fastest combination with standard algorithms, before/after measurements, final release.
