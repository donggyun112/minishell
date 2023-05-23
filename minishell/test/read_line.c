# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>

// 한줄을 읽고 메모리에 저장한 후 그 메모리를 반환

int main(void)
{
	char	*line;

	while (1)
	{
		line = readline(">>");
	}
}