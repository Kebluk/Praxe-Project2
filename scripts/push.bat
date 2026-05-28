@echo off
git checkout local
git fetch origin
git rebase origin/main
git push origin local:main --force-with-lease
pause