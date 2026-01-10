#pragma once 

#define SIZE 1024

#define FLUSH_NO    0
#define FLUSH_LINE  1
#define FLUSH_FULL  2

typedef struct{
  int fileno;       // 对应fd
  int flags;        // 缓冲类型
  int cap;
  int size;
  char outbuf[SIZE];
}mFILE;

mFILE* mfopen(const char* path, const char* mode);
void mffulsh(mFILE* stream);
int mfwrite(const void* ptr, int size, mFILE* stream);
void mfclose(mFILE* stream);

