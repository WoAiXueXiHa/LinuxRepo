#include "mystdio.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


IO_FILE* mfopen(const char* filename, const char* mode){
  int fd = -1;
  if(strcmp(mode,"r") == 0){
    fd = open(filename,O_RDONLY);
  }else if(strcmp(mode,"w") == 0){
    fd = open(filename,O_CREAT | O_WRONLY | O_TRUNC, 666);
  }else if(strcmp(mode,"a") == 0){
    fd = open(filename,O_CREAT | O_WRONLY | O_APPEND, 666);
  }

  if(fd < 0) return NULL;
  
  mFILE* mf = (mFILE*)malloc(sizeof(mFILE));
  if(!mf){
    close(fd);
    return NULL;
  }

  mf->fileno = fd;
  mf->flag = FLUSH_LINE;
  mf->cap = SIZE;
  mf->size = 0;

  return mf;
}


void mfflush(mFILE* stream){

  if(stream->size > 0){
    // write in core buffer
    write(stream->fileno,stream->buffer,stream->size);
    // flush to outmachine
    fsync(stream->fileno);
    stream->size = 0;
  }
}


int mfwrite(const void* ptr, int num, mFILE* stream){
  // copy 
  memcpy(stream->buffer+stream->size, ptr, num);
  stream->size += num;

  // flush 
  if(stream->flag == FLUSH_LINE && stream->size > 0 && stream->buffer[stream->size-1]=='\n')
    mfflush(stream);

  return num;
}


void mfclose(mFILE* stream){
  if(stream->size > 0) mfflush(stream);
  close(stream->fileno);
}

