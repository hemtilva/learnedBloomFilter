@echo off

cd /d "%~dp0BloomModule"

cmake --build build

copy /Y build\cppbloom.cp311-win_amd64.pyd ..\cppbloom.cp311-win_amd64.pyd

cd ..