#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

int	main(void)
{
	char	*str;

	while (1)
	{
		str = readline("prompt : ");
		if (!str)
			break ;
		rl_on_new_line();
		rl_replace_line(); // ----> 사용가능성을 모르겠음
		rl_redisplay(); // ----> 무엇에 쓰이는지 모르겠음
		printf("\n");
		add_history(str);
		free(str);
	}
	system("leaks a.out; rm -rf a.out");
	return (0);
}