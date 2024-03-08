## MiniShell 파싱 과정 상세 설명

MiniShell 프로젝트에서 사용자 입력을 처리하는 파싱 과정은 복잡한 명령어 시퀀스를 해석하여 개별 명령어와 인자, 리디렉션 및 파이프로 구분하는 과정입니다. 다음은 `"echo "hi my name is dongkseo"| ls > a"` 명령어를 예로 들어 파싱 과정을 단계별로 설명합니다.

### 1단계: 첫 번째 스플릿

사용자 입력을 공백을 기준으로 스플릿하여 임시 배열 `tmp1`에 저장합니다. 이 때, 쿼트 내부의 공백은 구분하지 않습니다.

- `tmp1[0]`: echo
- `tmp1[1]`: "hi my name is dongkseo"|
- `tmp1[2]`: ls
- `tmp1[3]`: >
- `tmp1[4]`: a

### 2단계: 두 번째 스플릿

첫 번째 스플릿 결과를 기반으로 명령어, 인자, 파이프, 리디렉션 기호를 명확히 구분하여 리스트에 저장합니다.

- `list[0]`: echo
- `list[1]`: "hi my name is dongkseo"
- `list[2]`: |
- `list[3]`: ls
- `list[4]`: >
- `list[5]`: a

### 3단계: 세 번째 스플릿

리스트의 각 요소에 대해 데이터 타입을 지정합니다. 여기서 데이터 타입은 명령어, 인자, 파이프, 리디렉션 등을 나타냅니다.

- `node[0]->data`: echo, `node[0]->type`: 명령어
- `node[1]->data`: "hi my name is dongkseo", `node[1]->type`: 인자
- `node[2]->data`: ls, `node[2]->type`: 명령어
- `node[3]->data`: a, `node[3]->type`: 파일

### 4단계: 네 번째 스플릿

최종적으로, 명령어와 관련된 데이터를 명령어 리스트 `cmd_list`에 저장하며, 각 명령어의 입력 파일(`infile`), 출력 파일(`outfile`)을 지정합니다.

- `cmd_list->cmd[0]`: echo, `cmd_list->infile[0]`: 표준 입력, `cmd_list->outfile[0]`: 표준 출력
- `cmd_list->cmd[1]`: "hi my name is dongkseo", `cmd_list->infile[1]`: 표준 입력, `cmd_list->outfile[1]`: 표준 출력
- `cmd_list->cmd[2]`: ls, `cmd_list->infile[2]`: 표준 입력, `cmd_list->outfile[2]`: 파일 `a` (리디렉션 대상)

이 과정을 통해 MiniShell은 사용자의 복잡한 입력을 개별 명령어와 인자로 분리하고, 리디렉션 및 파이프 연결을 정확하게 설정합니다. 이러한 파싱 과정은 명령어 실행을 위한 필수적인 단계로, 사용자 입력을 효과적으로 처리할 수 있게 합니다.
