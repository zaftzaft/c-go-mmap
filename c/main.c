#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>


struct hoge_t {
  int num;
  int num2;
};

int main(void) {
  int fd;
  int ret;
  int *mmap_ptr;
  struct stat stat;
  struct hoge_t *hoge;

  if((fd = shm_open("c-go-mmap", O_RDWR|O_CREAT, 0664)) == -1) {
    fprintf(stderr, "shm_open error\n");
		goto error;
  }

  ret = ftruncate(fd, sizeof(struct hoge_t));
  if(ret != 0) {
    fprintf(stderr, "ftruncate %s\n", strerror(errno));
		goto error;
  }

  fstat(fd, &stat);

  mmap_ptr = mmap(NULL, stat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  hoge = (struct hoge_t *)mmap_ptr;

  hoge->num = 0;
  hoge->num2 = 10;

	while(1) {
		hoge->num++;
		usleep(100 * 1000);
	}


error:
  close(fd);

  return 0;
}
