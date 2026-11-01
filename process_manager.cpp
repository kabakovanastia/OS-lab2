#include "process_manager.h"
#include <iostream>

#ifdef _WIN32
    #include <string>
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <cstdlib>
#endif

ProcessManager::ProcessManager() : is_running(false), exit_code(-1) {}

ProcessManager::~ProcessManager() {
#ifdef _WIN32
    if (is_running) {
        CloseHandle(handle.hProcess);
        CloseHandle(handle.hThread);
    }
#endif
}

bool ProcessManager::start(const char* executable, char* const argv[]) {
#ifdef _WIN32
    std::string cmdLine = executable;
    for (int i = 1; argv[i] != nullptr; ++i) {
        cmdLine += " ";
        cmdLine += argv[i];
    }

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&handle, sizeof(handle));

    if (!CreateProcessA(
            NULL,
            const_cast<char*>(cmdLine.c_str()),
            NULL, NULL, FALSE,
            0, NULL, NULL,
            &si, &handle)) {
        return false;
    }
    is_running = true;
    return true;

#else
    handle = fork();
    
    if (handle == -1) {
        return false;
    } else if (handle == 0) {
        execvp(executable, argv);
        perror("execvp failed");
        exit(1);
    }
    
    is_running = true;
    return true;
#endif
}

int ProcessManager::wait() {
    if (!is_running) return exit_code;

#ifdef _WIN32
    WaitForSingleObject(handle.hProcess, INFINITE);
    
    DWORD dwExitCode;
    GetExitCodeProcess(handle.hProcess, &dwExitCode);
    exit_code = static_cast<int>(dwExitCode);
    
    CloseHandle(handle.hProcess);
    CloseHandle(handle.hThread);
    
#else
    int status;
    waitpid(handle, &status, 0);
    
    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else {
        exit_code = -1;
    }
#endif

    is_running = false;
    return exit_code;
}

int ProcessManager::get_exit_code() const {
    return exit_code;
}