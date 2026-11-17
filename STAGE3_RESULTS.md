# Stage 3 baseline measurements

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
