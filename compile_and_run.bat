@echo off
echo.
echo ===========================================
echo   PyGui C++ Compilation and Execution Script
echo ===========================================
echo.

echo [1/2] Compiling C++ application with g++...
g++ -std=c++17 -I./cpp_library cpp_library/PyGui.cpp example/main.cpp -o example_app.exe

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Compilation failed! Please check the errors above.
    pause
    exit /b %errorlevel%
)

echo.
echo [2/2] Compilation successful!
echo Running example_app.exe...
echo.

example_app.exe

echo.
pause
