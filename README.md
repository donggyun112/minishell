# Minishell

## 소개
Minishell 프로젝트는 유명한 Unix 셸인 Bash의 기본적인 기능들을 모방하여 구현한 것입니다. 이 프로젝트를 통해 사용자는 시스템 호출, 프로세스 생성 및 관리 등 Unix 시스템 프로그래밍의 핵심 개념을 실습하고 이해할 수 있습니다. Minishell은 사용자로부터 커맨드를 입력 받고, 해당 커맨드를 실행한 결과를 출력하는 방식으로 동작합니다. 이 과정에서 환경 변수 관리, 신호 처리(signal handling), 파일 입출력 리다이렉션 등 Bash 셸의 주요 기능들이 구현되어 있습니다.

## 기능
Minishell은 다음과 같은 Bash의 기본 기능들을 지원합니다:

- 커맨드 실행
- 파일을 이용한 입출력 리다이렉션(`<`, `>`, `>>`)
- 파이프를 통한 커맨드 체이닝(`|`)
- 환경 변수(expand variables, `$`)
- 신호 처리(`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)
- 내장 커맨드의 구현(`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)

## 시작하기
### 의존성 설치
Minishell을 실행하기 전에, 리드라인 라이브러리를 포함한 몇 가지 의존성을 설치해야 합니다. 다음 명령어를 통해 필요한 의존성을 설치할 수 있습니다(Debian/Ubuntu 기준):

```bash
sudo apt-get update
sudo apt-get install -y libreadline-dev
