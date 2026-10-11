#include "NBPlayerController.h"
#include "NBGameModeBase.h"
#include "NBPlayerState.h"
#include "NBChatInput.h"
#include "Kismet/GameplayStatics.h"

void ANBPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어에게만 UI 생성
    if (!IsLocalController())
    {
        return;
    }

    if (ChatInputWidgetClass)
    {
        ChatInputWidget = CreateWidget<UNBChatInput>(
            this, ChatInputWidgetClass);

        if (ChatInputWidget)
        {
            ChatInputWidget->AddToViewport();

            bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(
                ChatInputWidget->TakeWidget());

            SetInputMode(InputMode);
        }
    }
}

void ANBPlayerController::ServerRPC_SendChatMessage_Implementation(
    const FString& Message)
{
    ANBGameModeBase* GameMode =
        Cast<ANBGameModeBase>(UGameplayStatics::GetGameMode(this));

    ANBPlayerState* NBPlayerState =
        GetPlayerState<ANBPlayerState>();

    if (!GameMode || !NBPlayerState)
    {
        return;
    }

    // 게임 종료 후 초기화 대기
    if (GameMode->IsGameFinished())
    {
        ClientRPC_ShowResult(TEXT("Game is restarting. Please wait!"));
        return;
    }

    // 입력값 검증
    if (!GameMode->IsValidGuess(Message))
    {
        ClientRPC_ShowResult(TEXT("Invalid Guess! Enter 3 unique digits (1-9)."));
        return;
    }

    // 시도 횟수 제한
    if (!NBPlayerState->CanAttempt())
    {
        ClientRPC_ShowResult(TEXT("No attempts remaining!"));
        return;
    }

    // 시도 횟수 증가
    NBPlayerState->AddAttempt();

    // 서버에서 스트라이크 / 볼 판정
    const FString Result = GameMode->JudgeGuess(Message);

    // 플레이어에게 판정 결과와 시도 횟수 전송
    const FString ResultMessage = FString::Printf(
        TEXT("%s -> %s | Attempts: %d/%d"),
        *Message,
        *Result,
        NBPlayerState->GetCurrentAttempts(),
        NBPlayerState->GetMaxAttempts()
    );

    ClientRPC_ShowResult(ResultMessage);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s"),
        *ResultMessage
    );

    // 승리 판정
    if (Result == TEXT("3S 0B"))
    {
        GameMode->HandleWin(this);
        return;
    }

    // 무승부 판정
    GameMode->CheckDraw();
}

void ANBPlayerController::ClientRPC_ShowResult_Implementation(
    const FString& Message)
{
    if (ChatInputWidget)
    {
        ChatInputWidget->ShowResult(Message);
    }
}

void ANBPlayerController::ClientRPC_ShowGameMessage_Implementation(
    const FString& Message)
{
    if (ChatInputWidget)
    {
        ChatInputWidget->ShowGameMessage(Message);
    }
}
