@echo off
rem Thin wrapper: configure with Ninja and build. Requires cmake, ninja and
rem the Arm GNU toolchain on PATH (see check_build_env.py).
setlocal
cd /d "%~dp0"
python check_build_env.py
if errorlevel 1 exit /b 1
cmake -S . -B build -G Ninja "-DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake"
if errorlevel 1 exit /b 1
cmake --build build
exit /b %errorlevel%
