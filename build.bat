@echo off
g++ -std=c++17 -Wall -Wextra -pedantic -c main.cpp BloomFilter.cpp
if errorlevel 1 exit /b %errorlevel%

g++ main.o BloomFilter.o -o app.exe
if errorlevel 1 exit /b %errorlevel%

app.exe