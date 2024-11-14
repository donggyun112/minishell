## 파이프 연결 과정 상세 설명

파이프 연결 과정은 명령어 사이의 데이터 흐름을 관리하기 위한 중요한 메커니즘입니다. 이 과정은 다음과 같은 핵심 단계로 구성됩니다.

### 기본 파이프 연결 설정

1. 프로세스를 분기하여 자식 프로세스와 부모 프로세스를 생성합니다.
2. 자식 프로세스에서는 표준 입력을 특정 입력 소스로 리디렉션하고, 표준 출력을 파이프의 쓰기 끝으로 리디렉션합니다. 출력이 다른 목적지로 지정된 경우, 해당 목적지로 표준 출력을 리디렉션하고, 파이프의 쓰기 끝을 닫습니다. 항상 파이프의 읽기 끝을 닫습니다.
3. 부모 프로세스에서는 필요한 경우 입력 소스를 닫습니다. 파이프의 쓰기 끝을 닫고, 다음 명령어의 입력 소스가 지정되지 않은 경우 파이프의 읽기 끝을 다음 명령어의 입력 소스로 설정합니다.

### 마지막 명령어 파이프 설정

마지막 명령어의 경우, 파이프 연결이 조금 달라집니다.

1. 자식 프로세스에서는 표준 입력과 출력을 각각 지정된 입력 소스와 출력 목적지로 리디렉션합니다. 파이프의 두 끝을 모두 닫습니다.
2. 부모 프로세스에서는 파이프의 두 끝을 닫습니다.

### 내장 명령어 처리

내장 명령어를 처리할 때는, 특히 마지막 명령어가 내장 명령어인 경우에, 별도의 프로세스 생성 없이 현재 셸 프로세스에서 명령어를 실행합니다.

1. 부모 프로세스에서 표준 입력과 출력을 각각 지정된 입력 소스와 출력 목적지로 리디렉션합니다. 파이프의 두 끝을 닫습니다. 추가적으로, 표준 출력이 기본값과 다를 경우 해당 출력 목적지를 닫습니다. 입력 소스도 닫습니다.
2. 자식 프로세스는 생성되지 않으므로, 파이프의 두 끝을 닫는 작업만 수행합니다.

### 연결 시퀀스 처리

전체 명령어 시퀀스에 대한 파이프 연결 처리는 다음과 같습니다.

- 각 명령어에 대해 파이프를 생성하고, 위에서 설명한 대로 파이프 연결 설정을 수행합니다.
- 명령어가 마지막인지, 다음 명령어가 있는지, 마지막 명령어가 내장 명령어인지를 판단하여, 상황에 맞게 파이프 연결 설정을 조정합니다.

이 과정을 통해, 파이프를 사용한 명령어 사이의 데이터 흐름을 효과적으로 관리할 수 있습니다. 각 명령어의 실행 결과는 다음 명령어의 입력으로 연속적으로 전달되며, 사용자는 복잡한 데이터 처리 작업을 간단한 명령어 조합으로 수행할 수 있게 됩니다.

### 상세구현
pipe_sequendce.c
```c

#include "../minishell.h"

void	set_pipe(pid_t pid, t_command *tmp, int fd[])
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		if (tmp->outfile == 1)
			dup2(fd[1], STDOUT_FILENO);
		else
		{
			dup2(tmp->outfile, STDOUT_FILENO);
			close(fd[1]);
		}
		close(fd[0]);
	}
	else
	{
		if (tmp->infile != 0)
			close(tmp->infile);
		close(fd[1]);
		if (tmp->next->infile == 0)
			tmp->next->infile = fd[0];
	}
}

void	set_pipe_last(pid_t pid, t_command *tmp, int fd[])
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		dup2(tmp->outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	set_pipe_builtin_last(pid_t pid, t_command *tmp, int fd[])
{
	if (pid > 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		dup2(tmp->outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (tmp->outfile != STDOUT_FILENO)
			close(tmp->outfile);
		close(tmp->infile);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	pipe_sequence(pid_t pid, t_command *tmp, t_fd *fds)
{
	if (tmp->next != NULL)
		set_pipe(pid, tmp, fds->fd);
	else if (tmp->next == NULL && check_builtin(tmp) && tmp->num_of_cmd == 0)
		set_pipe_builtin_last(pid, tmp, fds->fd);
	else if (tmp->next == NULL)
		set_pipe_last(pid, tmp, fds->fd);
}
```
