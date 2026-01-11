@echo off
cd /d "%~dp0.."

REM Arguments
set LANGUAGE=%1
set PROBLEM_ID=%2
set TITLE=%3 %4 %5 %6 %7 %8 %9

python scripts\leetcode_push.py %LANGUAGE% %PROBLEM_ID% "%TITLE%"

pause
