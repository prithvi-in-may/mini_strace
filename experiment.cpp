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

    // argv[1] gives value at index 1, &argv[1] gives value of memory address at index 1 which is an array
    execvp(argv[1], &argv[1]);
    
    // If execlp fails 
    perror("execlp");

  } else if (pid > 0) {
    // Parent process

    cout << "I am the parent" << endl;
    waitpid(pid, nullptr, 0);

  } else {
    perror("Fork");
  }
  
  return 0;

}

