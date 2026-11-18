# Task 3 requirements and evidence

| Requirement read from Moodle | Evidence |
|---|---|
| Extend v0.25 | Git history retains v0.25 and every progress commit. |
| Sort and group vector/list/deque | sortStudents, splitStudents, dispatch in main.cpp; menu option 3. |
| Strategy 1: retain original and create two groups | baseline and algorithms implementations; source is unchanged. |
| Strategy 2: only failures in new group, passed in original | compaction/erase or list node transfer; original logical size decreases. |
| Compare speed by container/strategy | results/baseline.csv: all 6 cases at 1K, 10K, 100K, 1M, 10M. |
| Investigate memory | recorded resident/peak memory; interpretation and limits in RESULTS.md. |
| Choose fastest combination and refactor with algorithms | list + strategy 2 selected from 10M survey; find_if + splice; other implementations use copy_if/remove_copy_if/remove_if. |
| Compare after refactoring | results/optimization.csv: 3 repetitions of before/after at all 5 sizes; median comparisons in README.md. |
| Clean repository | source, build files, docs, tests and small measured results; no executables/large datasets/IDE build files tracked. |
| README releases, results, usage from Make | README.md. |
| Makefile and CMakeLists.txt | both supplied and verified on Linux; Windows instructions included. |
| Synchronize v1.0 and default branch, publish release | prepared commands in FINISH_IN_VSCODE.md and scripts/publish.ps1; must be executed using the owner's GitHub authentication. Repository uses main, not master. |
| Submit to Moodle | submit published release link through Task 3; not done by creating local files. |

The last two rows are publishing/submission steps, not completed actions. No final grade is guaranteed by this checklist. Measurement data was recorded on 9 November 2026; current code/build correctness checks were rerun for packaging.
