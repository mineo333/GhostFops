#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
int main(){
  int fd;
  char* path = "/dev/null";
  struct stat* statbuf = malloc(sizeof(struct stat));
  while(1){
    /*if(*path == 'Q'){
      break;
    }*/
    //fd = open(path, O_WRONLY);
    //printf("%s",path);
    //write(1,path,11);
    //close(fd);
    stat(path, statbuf);


  }
  //close(fd);

  return 0;
}
