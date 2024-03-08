# MiniShell 프로젝트

MiniShell은 Bash 셸의 핵심 기능을 모델로 한 가벼운 커맨드 라인 인터페이스 프로젝트입니다. 이 프로젝트의 목적은 Unix/Linux 환경에서 셸의 기본적인 동작 원리를 이해하고, 사용자 정의 셸 환경을 구현하는 것입니다. MiniShell은 사용자로부터 커맨드를 입력 받아, 해당 커맨드를 실행하고 결과를 출력하는 기능을 제공합니다.

## 주요 기능

- **기본 셸 명령어 실행**: `ls`, `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit` 등의 Bash 셸 기본 명령어 실행 지원.
- **파이프라인(Pipes)과 리디렉션**: 명령어 간 파이프(`|`) 연결 지원 및 표준 입출력 리디렉션(`>`, `>>`, `<`) 기능 구현.
- **환경 변수 관리**: 환경 변수 설정 및 제거, 로컬 변수를 활용한 명령어 실행 지원.
- **시그널 처리**: `Ctrl+C`, `Ctrl+D`, `Ctrl+\` 시그널에 대한 사용자 정의 처리 구현.

## 시작하기

MiniShell 프로젝트의 소스 코드를 다운로드하고 컴파일하여 사용할 수 있습니다. 소스 코드는 C 언어로 작성되었으며, `gcc` 컴파일러를 사용하여 컴파일할 수 있습니다.

### 컴파일 방법

1. 소스 코드를 로컬 시스템에 클론합니다:
    ```bash
    git clone https://github.com/your-username/MiniShell.git
    ```
2. 프로젝트 디렉토리로 이동합니다:
    ```bash
    cd MiniShell
    ```
3. `Makefile`을 사용하여 프로젝트를 컴파일합니다:
    ```bash
    make
    ```
4. 컴파일된 MiniShell 실행 파일을 실행하여 셸을 시작합니다:
    ```bash
    ./minishell
    ```

## 사용 예제

MiniShell에서는 기본 Bash 셸 명령어를 실행할 수 있습니다. 예를 들어, 현재 디렉토리의 파일 목록을 보려면 다음과 같이 입력합니다:
```bash
ls
```
또한, 여러 명령어를 파이프로 연결하여 사용할 수 있습니다
```bash
ls | grep "minishell"
```

## 추가 자료

본 프로젝트의 더 상세한 정보를 원하시면, 아래 링크들을 참조해 주세요.

- [파이프 연결과 파싱 과정 상세](./execute/README.md)
- [명령어 실행 및 환경 설정 상세](./parse/README.md)

