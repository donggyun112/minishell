#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>

// 정상적으로 다음 빈 개행라인으로 움직였는지 확인하는 함수

int main(void)
{
	char *a;

	a = readline("input :");
	printf("%d\n" ,rl_on_new_line());
	printf("%s\n", a);
	printf("%d\n" ,rl_on_new_line());
	free(a);
}