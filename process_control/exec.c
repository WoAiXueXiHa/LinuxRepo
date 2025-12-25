#include <stdio.h>
#include <unistd.h>

//int main(){
 // execl("/bin/ls","ls","-al","--color",NULL);
//  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//    char* argv[] = {"ls","-l",NULL};
//  execv("/bin/ls",argv);
//  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
 // execlp("ls","ls","--color",NULL);
// execvp("ls",argv);
//  return 0;
//}
int main(){
//  printf("我是 exec, pid: %d\n",getpid());
//  execl("./other",NULL);
    char* argv[] = {"ls","-l",NULL};
    execvpe("ls",argv,NULL);
  return 0;
}
