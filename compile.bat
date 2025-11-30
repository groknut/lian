



@echo off

@REM компиляция и запуск под windows

echo compile project...

g++ ic.cpp map.cpp lian.cpp main.cpp -o main

if %errorlevel% neq 0 (
    echo compile error!
    pause
    exit /b 1
)

.\main.exe config.ic

