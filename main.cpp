#include <cstdlib>
#include <cstring>
#include <csignal>
#include <iostream>
#include <cstdio>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char * argv[]) { 
  
  pid_t pid = fork();

  if (pid == 0) {
    // Child process

    char *args[100];

    cout << "I am the child" << endl;

    // Using ptrace option PTRACE_TRACEME to allow the parent process trace the child process and puts a `tracing stop` after execvp is executed 

    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);

    // argv[1] gives value at index 1, &argv[1] gives value of memory address at index 1 which is an array
    execvp(argv[1], &argv[1]);
    // After the function is executed, Kernel will stop it with `SIGTRAP` signal as part of tracing mechanism.
    
     // If execlp fails 
    perror("execlp");

  } else if (pid > 0) {
    // Parent process

    cout << "I am the parent" << endl;

    // Status stores what state change happened to the child
    
    int status;
    waitpid(pid, &status, 0);

    // WSTOPSIG macro extracts the signal responsible for the change in state. strsignal converts that signal in readable format

    cout << "Signal: " << WSTOPSIG(status);
    cout << " (" << strsignal(WSTOPSIG(status)) << ")" << endl;

  } else {
    perror("Fork");
  }
  
  return 0;

}

