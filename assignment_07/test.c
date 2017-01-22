#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	char result[10];
	int fd;
	ssize_t ret;
	printf("id:\n");
	if ((fd = open("/sys/kernel/debug/fortyfor/id", O_RDWR)) == -1)
	{
		printf("open() failed\n");
		return (EXIT_FAILURE);
	}
	memset(result, 0, 10);
	if (read(fd, result, 10) != 6)
	{
		printf("read() != 6\n");
		return (EXIT_FAILURE);
	}
	printf("result: %s\n", result);
	ret = write(fd, result, 6);
	printf("write(\"acazuc\") = %ld, errno = %d\n", ret, errno);
	result[0] = 'c';
	ret = write(fd, result, 6);
	printf("write(\"ccazuc\") = %ld, errno = %d\n", ret, errno);
	close(fd);
	printf("\n\njiffies:\n");
	if ((fd = open("/sys/kernel/debug/fortyfor/jiffies", O_RDONLY))
			== -1)
	{
		printf("open() failed\n");
		return (EXIT_FAILURE);
	}
	unsigned long jiffies;
	if (read(fd, &jiffies, 8) != 8)
	{
		printf("read() != 8\n");
		return (EXIT_FAILURE);
	}
	printf("jiffies: %lu\n", jiffies);
	return (EXIT_SUCCESS);
}
