@echo off
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp BloomFilter.cpp Evaluator.cpp -o app.exe
if errorlevel 1 exit /b %errorlevel%