# Stage 3 — approximately 88%

This complete source package includes your previous work plus this checkpoint. Do not paste its main.cpp below existing code. Import the numbered patches in commits/ into your existing task3-repository, on a clean task3-progress branch. There are three new commits.

Extract the ZIP separately as C:\Users\sarae\OneDrive\Desktop\task3-stage3, then run:

```powershell
cd C:\Users\sarae\OneDrive\Desktop\task3-repository
git status
```

When clean and on task3-progress:

```powershell
git am (Get-ChildItem "C:\Users\sarae\OneDrive\Desktop\task3-stage3\commits\*.patch" | Sort-Object Name | Select-Object -ExpandProperty FullName)
git log -3 --oneline
```

Stop if any command fails. After checking the imported commits, publish progress with git push origin task3-progress. The final v1.0 release and Moodle submission remain for the next stage. See README.md for compilation and usage.
