
@echo off

@REM запуск под windows

echo run project...

echo %time% 
uv run ./py/create-map.py ./input/karta-01.bmp

echo %time%
.\main.exe config.ic
echo %time%
uv run ./py/generate-map.py ./input/karta-01.bmp
