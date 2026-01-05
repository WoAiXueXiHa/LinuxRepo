#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>     // chdir fork exec系列 getcwd dup2 close
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid
#include <ctype.h>      // isspace

using namespace std;

const int gBaseSize = 1024;
const int gArgvNum = 64;
const int gEnvNum = 64;

// 解析命令行存到数组
char* gArgv[gArgvNum];
int gArgc = 0;

// 存上一个命令的退出码 $?
int gLastCode = 0;

// 存shell的环境变量
char* gEnv[gEnvNum];

// 存shell的cwd
char pwd[gBaseSize];
char pwdEnv[gBaseSize];   // pwd环境变量缓冲区

// 重定向宏
#define NoneRedir   0
#define InputRedir  1
#define OutputRedif 2
#define AppendRedir 3

// 记录当前重定向类型和目标文件
int gRedir = NoneRedir;
char* gFilename = nullptr;

// do{...}while(0) 包裹保证宏作物独立语句执行
#define TrimSpace(pos) do{ while(isspace(*pos)){ pos++;} }while(0)

// 初始化环境变量：从系统的environ拷贝到自定义的gEnv数组
void InitEnv(){
  extern char** environ;
  int index = 0;
  while(environ[index]){
    gEnv[index] = (char*)malloc(strlen(environ[index])+1);
    strncpy(gEnv[index],environ[index],strlen(environ[index])+1);
    index++;
  }
  gEnv[index] = nullptr; // 末尾置空
}

// ========== 1. 打印命令行提示符 =========
string GetUserName(){
  string name = getenv("USER");
  return name.empty() ? "None" : name;
}
string GetHostName(){
  string hostname = getenv("HOSTNAME");
  return hostname.empty() ? "None" : hostname;
}
string GetPwd(){
  if(nullptr == getcwd(pwd,sizeof(pwd))) return "None";
  snprintf(pwdEnv,sizeof(pwdEnv),"PWD=%s",pwd); // 拼接pwd
  putenv(pwdEnv); // 同时更新系统的pwd环境变量
  return pwd;
}
string LastDir(){
  string cur = GetPwd();
  if(cur == "/" || cur == "None") return cur;
  size_t pos = cur.rfind("/");
  return pos == string::npos ? cur : cur.substr(pos+1);
}
void PrintCommandLine(){
  char conmand_line[gBaseSize];
  // 拼接提示符 [用户名@主机名 最后一级目录]&
  snprintf(command_line,gBaseSize,"[%s@%s %s]&",
          GetUserName().c_str(), GetHostName.c_str(), LastDir.c_str());
  printf("%s",command_line);
  fflush(stdout);
}

// ============= 2. 获取用户输入 ===========
bool GetCommandLine(char command_buffer[], int size){
  char* result = fgets(command_buffer,size,stdin);
  if(result == nullptr) return false;

  // 去掉末尾的换行符 fgets会读取"\n"
  command_buffer[strlen(command_buffer) - 1] = '\0';

  // 过滤空输入 用户只按回车
  if(strlen(command_buffer) == 0) return false;

  return true;
}

// ============= 3. 解析命令行  ===========
void ParseCommandLine(char command_buffer, int size){
  
}

