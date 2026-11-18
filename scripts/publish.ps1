# Run after importing final patches, from the existing repository.
$ErrorActionPreference = 'Stop'
function Run-Git {
    & git @args
    if ($LASTEXITCODE -ne 0) { throw "Git failed; stop and inspect the output before continuing." }
}
$branch = git branch --show-current
if ($LASTEXITCODE -ne 0 -or $branch -ne 'task3-progress') { throw 'Run from task3-progress in your existing repository.' }
if (git status --porcelain) { throw 'Commit or preserve local changes before publishing.' }
Run-Git fetch origin
Run-Git merge-base --is-ancestor origin/task3-progress HEAD
$mainHead = git rev-parse origin/main
if ($mainHead -ne '53dbf050d91f30d97178bf89bdca942dc2627e4b') {
    throw 'main has changed since review. Stop and review the new changes before merging.'
}
if (git branch --list v1.0) { throw 'Local v1.0 branch already exists; inspect it before continuing.' }
if (git tag --list v1.0) { throw 'v1.0 tag already exists; do not overwrite a release.' }
$remoteRelease = git ls-remote origin refs/heads/v1.0 refs/tags/v1.0
if ($LASTEXITCODE -ne 0) { throw 'Could not check remote releases.' }
if ($remoteRelease) { throw 'Remote v1.0 already exists; inspect before continuing.' }
# The final source incorporates filename input from the old main branch.
# Preserve its ancestry while retaining the completed Task 3 implementation.
Run-Git merge -s ours origin/main -m 'Preserve previous main history in completed v1.0'
Run-Git push origin task3-progress
Run-Git switch -c v1.0
Run-Git push -u origin v1.0
Run-Git switch main
Run-Git merge --ff-only origin/main
Run-Git merge --ff-only v1.0
Run-Git push origin main
Run-Git tag -a v1.0 -m 'Complete Systems Programming Task 3'
Run-Git push origin refs/tags/v1.0
Write-Host 'Branches and tag published. Next create the GitHub Release and submit its URL to Moodle (see FINISH_IN_VSCODE.md).'
