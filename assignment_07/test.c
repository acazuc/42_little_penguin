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
	if ((ret = read(fd, result, 10)) != 6)
	{
		printf("read(10) = %ld (expected 6)\n", ret);
		return (EXIT_FAILURE);
	}
	printf("OK\n");
	printf("result: %s\n", result);
	if ((ret = write(fd, result, 6)) != 6)
	{
		printf("write(\"acazuc\", 6) = %ld, errno = %d (expected 6)"
				"\n", ret, errno);
		return (EXIT_FAILURE);
	}
	printf("OK\n");
	result[0] = 'c';
	if ((ret = write(fd, result, 6) != -1))
	{
		printf("write(\"ccazuc\", 6) = %ld (expected -1)\n", ret);
		return (EXIT_FAILURE);
	}
	printf("OK\n");
	close(fd);
	printf("\n\njiffies:\n");
	if ((fd = open("/sys/kernel/debug/fortyfor/jiffies", O_RDONLY))
			== -1)
	{
		printf("open() failed\n");
		return (EXIT_FAILURE);
	}
	unsigned long jiffies;
	if ((ret = read(fd, &jiffies, 9)) != 8)
	{
		printf("read(9) = %ld (expected 8)\n", ret);
		return (EXIT_FAILURE);
	}
	printf("OK\n");
	printf("jiffies: %lu\n", jiffies);
	printf("\n\nfoo:\n");
	if ((fd = open("/sys/kernel/debug/fortyfor/foo", O_RDWR))
			== -1)
	{
		printf("open() failed\n");
		return (EXIT_FAILURE);
	}
	ssize_t len = 0;
	char buffer[13] = "Yeah!acazuc\n";
	char c;
	if ((ret = read(fd, &c, 1)) != 0)
	{
		++len;
		printf("/!\\ BUFFER NOT EMPTY (did you already run the"
				" test ?)\n");
		while ((ret = read(fd, &c, 1)))
		{
			++len;
			if (ret == -1)
			{
				printf("read() failed\n");
				return (EXIT_FAILURE);
			}
		}
	}
	printf("OK\n");
	if ((ret = write(fd, buffer, 13)) != 13)
	{
		printf("write(\"Yeah!acazuc\\n\", 13) = %ld (expected 13)\n"
				, ret);
		return (EXIT_FAILURE);
	}
	printf("OK\n");
	memset(buffer, 0, 13);
	if ((fd = open("/sys/kernel/debug/fortyfor/foo", O_RDONLY)) == -1)
	{
		printf("open() failed\n");
		return (EXIT_FAILURE);
	}
	char tmp[len];
	if (read(fd, tmp, len) != len)
	{
		printf("failed to emtpy old buffer\n");
		return (EXIT_FAILURE);
	}
	if ((ret = read(fd, buffer, 14)) != 13)
	{
		printf("read(14) = %ld (expected 13)\n", ret);
		return (EXIT_FAILURE);
	}
	printf("buffer: %s\n", buffer);
	close(fd);
	return (EXIT_SUCCESS);
}
