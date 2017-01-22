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
	if ((fd = open("/dev/fortytwo", O_RDWR)) == -1)
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
	return (EXIT_SUCCESS);
}
