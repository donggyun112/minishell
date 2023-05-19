# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>

// 한줄을 읽고 메모리에 저장한 후 그 메모리를 반환

int main(void)
{
	char *arr[10];

	arr[0] = "ls";
	arr[1] = "\"asd\"";
	arr[2] = "-al";
	arr[3] = NULL;
	execve("/bin/ls", arr, NULL);
}