
@echo off

@REM запуск под windows

echo run project...
echo %time%
.\main.exe config.ic
echo %time%
uv run main.py
