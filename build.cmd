git pull origin main
cmake -S . -B build
cmake --build build
build\test_process.exe
pause
