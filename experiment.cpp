#include <iostream>
#include <cstdio>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using namespace std;

void tokenize(char input[], char* buf[]);

int main(int argc, char * argv[]) { 
  
  pid_t pid = fork();

  if (pid == 0) {
    // Child process

    char *args[100];

    cout << "I am the child" << endl;

    tokenize((char*)argv, args);
    execvp(argv[1], args);
    
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

void tokenize(char input[], char* buf[]) {

  char *token = strtok(input, " ");
  int i = 0;

  while(token != NULL) {
    buf[i++] = token;
    token = strtok(NULL, " ");
  }

  buf[i] = nullptr;

}
