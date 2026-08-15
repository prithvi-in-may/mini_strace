#include <iostream>
#include <cstdio>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() { 
  
  pid_t pid = fork();

  if (pid == 0) {
    // Child process

    cout << "I am the child" << endl;
    execlp("ls", "ls", "-l", "-a", nullptr);
    
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
