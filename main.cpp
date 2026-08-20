#include <cstdlib>
#include <cstring>
#include <csignal>
#include <iostream>
#include <cstdio>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/user.h>

using namespace std;
const char* syscall_name(long number);

int main(int argc, char * argv[]) {

  if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program> [args...]\n";
        return 1;
  }

  pid_t pid = fork();

  if (pid == 0) {
    // Child process

    // Using ptrace option PTRACE_TRACEME to allow the parent process trace the child process and puts a `tracing stop` after execvp is executed
    // ptrace second argument is the process id which must be passed in other options except PTRACE_TRACEME
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);

    // argv[1] gives value at index 1, &argv[1] gives value of memory address at index 1 which is an array
    execvp(argv[1], &argv[1]);
    // After the function is executed, Kernel will stop it with `SIGTRAP` signal as part of tracing mechanism.

     // If execlp fails
    perror("execlp");

  } else if (pid > 0) {
    // Parent process

    // Status stores what state change happened to the child
    int status;
    waitpid(pid, &status, 0);

    cout << "Signal: " << WSTOPSIG(status);
    cout << " (" << strsignal(WSTOPSIG(status)) << ")" << endl;

    // Allow the child to run before the parent dies, the child will run anyway if the tracer (parent process) disappears though
    bool entering_syscall = true; 
    struct user_regs_struct regs;
    while (true) {
    ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);

    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      break;
    }
    ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

    if (entering_syscall) {
      // cout << "SYSCALL ENTRY: " << syscall_name(regs.orig_rax) << endl;
      cout << syscall_name(regs.orig_rax) << "(" << regs.rdi << ", " << regs.rsi << ", " << regs.rdx << ", " << regs.r10 << ", " << regs.r8 << ", " << regs.r9 << ")" ;
    } else {
      cout << " = " << regs.rax << endl;
      // cout << "SYSCALL EXIT: " << syscall_name(regs.orig_rax) << endl;
      // cout << "Return value: " << regs.rax << endl; // Return value of syscall
    }

    entering_syscall = !entering_syscall; // It sarts with being true, when the first half cycle of syscall is completed, it flips to false to indicate exit point.

    }

  } else {
    perror("Fork");
  }

  return 0;

}

const char* syscall_name(long number) {
    switch (number) {
        case 0:   return "read";
        case 1:   return "write";
        case 2:   return "open";
        case 3:   return "close";
        case 5:   return "fstat";
        case 9:   return "mmap";
        case 10:  return "mprotect";
        case 11:  return "munmap";
        case 12:  return "brk";
        case 16:  return "ioctl";
        case 21:  return "access";
        case 39:  return "getpid";
        case 60:  return "exit";
        case 61:  return "wait4";
        case 158: return "arch_prctl";
        case 217: return "getdents64";
        case 231: return "exit_group";
        case 257: return "openat";

        default:
            return "unknown";
    }
}
