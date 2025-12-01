
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
uv run ./py/create-map.py ./input/karta-01.bmp

echo %time%
.\main.exe config.ic
echo %time%
uv run py/generate-map.py ./input/karta-01.bmp ./output/path.png
