#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

// 한줄을 읽고 메모리에 저장한 후 그 메모리를 반환

int main(void)
{
	char *a;

	while (1)
	{
		a = readline("\nbash: input :");
		if (!a)
			break ;
		printf("%s", a);
		free(a);
	}
}