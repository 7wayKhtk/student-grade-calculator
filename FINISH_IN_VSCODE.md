# Apply and publish the final Task 3 work

The final package includes the entire project plus numbered patches in commits/. The patches contain only the work added after the stage-3 commits already on GitHub.

1. Extract into a separate Desktop folder named task3-complete.
2. Open the existing task3-repository folder in VS Code. Run git status; it must be clean and on task3-progress.
3. Import the three final patches:

```powershell
git am (Get-ChildItem "C:\Users\sarae\OneDrive\Desktop\task3-complete\commits\*.patch" | Sort-Object Name | Select-Object -ExpandProperty FullName)
```

If this fails, stop and inspect the error. git am --abort restores the pre-import state if you choose to cancel. Do not copy the commits folder into your repository.

4. Compile and run (existing MinGW/g++ setup):

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp Student.cpp -o student_grade_calculator.exe
.\student_grade_calculator.exe
```

For the chosen optimized case, choose option 3, an input file, sorting/grade options, strategy 2 and standard algorithms. Option 3 compares all three containers. Option 1 processes vector only. Python checks: python scripts/check.py ./student_grade_calculator.exe.

5. Publish with the checked script:

```powershell
.\scripts\publish.ps1
```

Alternatively inspect and run its Git commands individually. The script checks the branch, clean working tree and expected old main commit, then preserves old main history, creates v1.0, fast-forwards main, and pushes branches and the annotated tag. It never force-pushes or rewrites old commits. If a step fails, stop; earlier successful pushes are retained.

6. Create the GitHub Release. If GitHub CLI is installed and signed in:

```powershell
gh release create v1.0 --verify-tag --title "Student Grade Calculator v1.0" --notes-file RELEASE_NOTES.md
```

Otherwise open your repository's Releases page, choose Draft a new release, select the existing v1.0 tag, use RELEASE_NOTES.md as the description and publish. GitHub automatically provides source ZIP/tar archives.

7. Submit the published release URL through Add submission on the Moodle Task 3 page, and complete any final submit step Moodle shows. Creating a tag or a GitHub Release alone does not submit to Moodle.

The teacher calls the default branch master; this repository uses main. The script updates main and a separate v1.0 branch. Old main has a filename-input change absent from the original v0.25 history; the final implementation already supports filename input. The checked merge deliberately retains the completed Task 3 tree while preserving that old history.
