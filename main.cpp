#include "process_manager.h"
#include <iostream>

int main() {
    ProcessManager pm;

#ifdef _WIN32
    std::cout << "Starting child process (Windows cmd)...\n";
    char* args[] = {
        const_cast<char*>("cmd.exe"), 
        const_cast<char*>("/c"), 
        const_cast<char*>("timeout /t 2 /nobreak > nul & exit 5"), 
        nullptr
    };
    if (!pm.start("cmd.exe", args)) {
        std::cerr << "Failed to start process\n";
        return 1;
    }
#else
    std::cout << "Starting child process (POSIX sleep)...\n";
    char* args[] = {
        const_cast<char*>("sleep"), 
        const_cast<char*>("2"), 
        nullptr
    };
    if (!pm.start("sleep", args)) {
        std::cerr << "Failed to start process\n";
        return 1;
    }
#endif

    std::cout << "Process started in background. Waiting for completion...\n";
    
    int code = pm.wait();
    
    std::cout << "Child process finished with exit code: " << code << "\n";

    return 0;
}