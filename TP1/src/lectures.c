#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define NB_PROC 3


int print_char_posix(const char *path) 
{
	int fd, i, lus, p;
	char c;

	if ((fd = open (path, O_RDONLY, 0600)) == -1) {
      	perror("open fd");
      	return errno;
    }

    for (i = 0; i < NB_PROC; ++i)
	{
		if((p = fork()) == -1)
			perror("fork error");
		else if (p == 0) {
			lus = read(fd, &c, 1);
			while(lus != 0) {
				printf("%d lit le char: %c\n", getpid(), c);
				lus = read(fd, &c, 1);
			}
			return EXIT_SUCCESS;
		}
	}

	for (i = 0; i < NB_PROC; ++i)
	{
		wait(NULL);
	}

	close(fd);
	
	return EXIT_SUCCESS;
}

int print_char_c(const char *path) 
{
	FILE *fd;
	int i, p;
	int c;

	if ((fd = fopen(path, "r")) == NULL) {
      	perror("fopen fd");
      	return errno;
    }

    for (i = 0; i < NB_PROC; ++i)
	{
		if((p = fork()) == -1)
			perror("fork error");
		else if (p == 0) {
			c = fgetc(fd);
			while(c != EOF) {
				printf("%d lit le char: %c\n", getpid(), c);
				c = fgetc(fd);
			}
			return EXIT_SUCCESS;
		}
	}

	for (i = 0; i < NB_PROC; ++i)
	{
		wait(NULL);
	}

	fclose(fd);
	
	return EXIT_SUCCESS;
}

int main(int argc, char const **argv)
{
	if (argc != 3)
    {
        printf("usage: %s [-p|-C] filepath\n", argv[0]);
        return EXIT_FAILURE;
    }

    if(strcmp(argv[1], "-p") == 0)
    	return print_char_posix(argv[2]);
    else if (strcmp(argv[1], "-C") == 0)
    	return print_char_c(argv[2]);
	else {
		printf("Error: Unknown mode\nusage: %s [-p|-C] filepath\n", argv[0]);
		return EXIT_FAILURE;
	}
}