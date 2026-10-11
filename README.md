# NumberBaseball ⚾

## 1. 프로젝트 소개

**Unreal Engine 5.5와 C++을 활용한 멀티플레이 숫자 야구 게임**입니다.

서버가 생성한 세 자리 숫자를 플레이어들이 추측하고, 스트라이크(S)와 볼(B) 판정을 통해 정답을 맞히는 게임이며

언리얼 엔진의 **RPC, GameMode, PlayerController, PlayerState, UMG**를 활용하여 서버 중심의 게임 로직과 클라이언트 UI를 구현했습니다.

## 2. 개발 환경

| 항목 | 내용 |
|---|---|
| 엔진 | Unreal Engine 5.5 |
| 언어 | C++ |
| IDE | Visual Studio 2022 |
| 버전 관리 | Git / GitHub Desktop |
| 플레이 방식 | 2인 멀티플레이 (Listen Server) |

## 3. 게임 규칙

- 서버가 1~9 사이의 서로 다른 숫자 3개를 무작위로 선택하여 정답을 생성합니다.
- 플레이어는 숫자 3자리를 입력하여 정답을 추측합니다.
- 숫자와 위치가 모두 일치하면 **Strike(S)**, 숫자만 일치하면 **Ball(B)**로 판정합니다.
- 일치하는 숫자가 하나도 없으면 **OUT**으로 판정합니다.
- 각 플레이어에게 최대 **3번의 시도 기회**가 주어집니다.
- 먼저 **3 Strike**를 달성한 플레이어가 승리합니다.
- 모든 플레이어가 기회를 소진하면 무승부로 처리합니다.
- 게임 종료 후 정답과 시도 횟수를 초기화하여 새로운 게임을 시작합니다.

### 입력 제한

정확히 세 자리의 숫자만 입력할 수 있으며, `0` 또는 중복된 숫자가 포함된 입력은 무효 처리됩니다.

잘못된 입력은 시도 횟수에 포함되지 않습니다.

## 4. 주요 구현 기능

### 서버 중심의 게임 로직

`NBGameModeBase`에서 정답 생성, 입력 검증, 스트라이크·볼 판정, 승패 및 무승부 처리, 게임 초기화를 담당합니다.

### RPC를 활용한 입력 처리

클라이언트가 입력한 숫자를 Server RPC를 통해 서버에 전달합니다. 서버에서 입력값을 검증하고 판정한 뒤, 결과를 클라이언트 UI에 반영합니다.

### PlayerState를 활용한 시도 횟수 관리

플레이어별 현재 시도 횟수와 최대 시도 횟수를 관리합니다. 유효한 입력에 대해서만 시도 횟수가 증가하며, 최대 횟수 도달 시 추가 입력을 제한합니다.

### UMG 기반 게임 UI

- 게임 제목 및 로고
- 마지막 입력 숫자 (`LAST GUESS`)
- 스트라이크 / 볼 판정 (`RESULT`)
- 남은 시도 기회를 확인할 수 있는 시도 횟수
- 숫자 입력창
- 잘못된 입력에 대한 안내 메시지
- 승리, 무승부, 새 게임 시작 메시지
- 게임 상태에 따른 메시지 색상 변경

## 5. 실행 방법

1. GitHub 저장소를 Clone하거나 ZIP으로 다운로드합니다.
2. `NumberBaseball.uproject` 파일을 Unreal Engine 5.5로 실행합니다.
3. 필요한 경우 Visual Studio 2022에서 C++ 프로젝트를 빌드합니다.
4. PIE 설정에서 **Number of Players를 2**로 설정합니다.
5. Net Mode를 **Play As Listen Server**로 설정합니다.
6. Play 버튼을 눌러 게임을 실행합니다.
7. 각 플레이어의 입력창에 서로 다른 세 자리 숫자를 입력하고 Enter를 눌러 게임을 진행합니다.

※ 시작 레벨은 `NumberBaseballMap`으로 지정되어 있습니다.

## 6. 테스트 내용

- 서버에서 중복되지 않는 세 자리 정답 생성 확인
- 정상 입력에 대한 Strike / Ball / OUT 판정 확인
- 잘못된 입력 시 시도 횟수가 증가하지 않는지 확인
- 플레이어별 최대 3회 입력 제한 확인
- 3 Strike 달성 시 승리 처리 확인
- 모든 플레이어의 기회 소진 시 무승부 처리 확인
- 게임 종료 후 새로운 정답 생성 및 시도 횟수 초기화 확인
- Listen Server 환경에서 2인 입력 및 결과 UI 표시 확인

## 7. 프로젝트 구조

```text
NumberBaseball/
├── Config/
├── Content/
│   ├── NumberBaseballMap.umap
│   └── ...
├── Source/
│   └── NumberBaseball/
│       ├── NBGameModeBase.h / .cpp
│       ├── NBPlayerController.h / .cpp
│       ├── NBPlayerState.h / .cpp
│       ├── NBChatInput.h / .cpp
│       └── ...
├── NumberBaseball.uproject
└── README.md
```


