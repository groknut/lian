
@echo off

@REM компиляция и запуск под windows

echo compile project...

echo %time% g++ ic.cpp map.cpp lian.cpp main.cpp -o main

if %errorlevel% neq 0 (
    echo compile error!
    pause
    exit /b 1
)

echo %time%
.\main.exe config.ic
echo %time%
uv run main.py
