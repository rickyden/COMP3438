#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void main(){
	int fd;
	char buf[255];
	int count;
	
	fd = open("/dev/echoword_1", O_WRONLY);
    if (fd == -1) {
        perror("Fail to open /dev/wordecho_1. \n");
        return;
    }
	while (1){
		printf("User input string: ");
        scanf("%s", &buf); //scan string to input ADDRESS
		write(fd, &buf, 255);//write input ADDRESS to fd
	}
	close(fd);
	return;
}