#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#ifdef _WIN32
    #include <windows.h>
    typedef PROCESS_INFORMATION ProcessHandle;
#else
    #include <sys/types.h>
    typedef pid_t ProcessHandle;
#endif

class ProcessManager {
public:
    ProcessManager();
    ~ProcessManager();

    bool start(const char* executable, char* const argv[]);

    int wait();

    int get_exit_code() const;

private:
    ProcessHandle handle;
    bool is_running;
    int exit_code;
};

#endif