#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

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

mFILE* mfopen(const char* path, const char* mode){
  int fd = -1;
  if(strcmp(mode,"r") == 0){
   fd = open(path,O_CREAT | O_RDONLY, 0666);
  }
  if(strcmp(mode,"w") == 0){
    fd = open(path,O_CREAT | O_WRONLY | O_TRUNC, 0666);
  }
  if(strcmp(mode,"a") == 0){
    fd = open(path,O_CREAT | O_APPEND | O_RDWR, 0666);
  }

  if(fd < 0) return NULL;

  mFILE* mf = (mFILE*)malloc(sizeof(mFILE));
  if(!mf){
    close(fd);
    return NULL;
  }

  mf->fileno = fd;
  mf->cap = SIZE;
  mf->size = 0;
  mf->flags = FLUSH_LINE;
}

void mffulsh(mFILE* stream){
  // 缓冲区有数据才刷新
  if(stream->size > 0){
    // 1. 写入内核缓冲区
    write(stream->fileno, stream->outbuf, stream->size);

    // 2. 强制刷新到终端
    fsync(stream->fileno);

    // 3. 重置缓冲区
    stream->size = 0;
  }
}

size_t mfwrite(const void* ptr, size_t size, mFILE* stream){
  // 1. 拷贝数据
  memcpy(stream->outbuf+stream->size, ptr, size);
  stream->size += size;

  // 2. 行缓冲检测 
  if(stream->flags == FLUSH_LINE && stream->size > 0 && stream->outbuf[stream->size - 1] == '\n')
    mffulsh(stream);

  return size;
}

void mfclose(mFILE* stream){
  if(stream->size > 0)
    mffulsh(stream);
  close(stream->fileno);
  free(stream);
}

int main(){
  mFILE* fp = mfopen("./test.txt","w");
  if(!fp){
    perror("err");
    return 1;
  }
  const char* msg = "hello stdio\n";
  mfwrite(msg,strlen(msg),fp);

  mfclose(fp);

  return 0;
}
