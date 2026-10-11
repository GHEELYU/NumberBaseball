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
            this, ChatInputWidgetClass
        );

        if (ChatInputWidget)
        {
            ChatInputWidget->AddToViewport();

            bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            SetInputMode(InputMode);

            ChatInputWidget->FocusChatInput();
        }
    }
}

void ANBPlayerController::ServerRPC_SendChatMessage_Implementation(
    const FString& Message)
{
    ANBGameModeBase* GameMode =
        Cast<ANBGameModeBase>(
            UGameplayStatics::GetGameMode(this)
        );

    ANBPlayerState* NBPlayerState =
        GetPlayerState<ANBPlayerState>();

    if (!GameMode || !NBPlayerState)
    {
        return;
    }

    // 게임 종료 후 재시작 대기
    if (GameMode->IsGameFinished())
    {
        ClientRPC_ShowResult(
            TEXT("Game is restarting. Please wait!")
        );
        return;
    }

    // 입력값 검증
    if (!GameMode->IsValidGuess(Message))
    {
        ClientRPC_ShowResult(
            TEXT("Invalid Guess! Enter 3 unique digits (1-9).")
        );
        return;
    }

    // 시도 횟수 제한
    if (!NBPlayerState->CanAttempt())
    {
        ClientRPC_ShowResult(
            TEXT("No attempts remaining!")
        );
        return;
    }

    // 유효한 입력일 때만 횟수 증가
    NBPlayerState->AddAttempt();

    // 서버에서 숫자 판정
    const FString Result = GameMode->JudgeGuess(Message);

    // 판정 결과를 해당 플레이어에게 전달
    ClientRPC_ShowGuessResult(
        Message,
        Result,
        NBPlayerState->GetCurrentAttempts(),
        NBPlayerState->GetMaxAttempts()
    );

    // 서버 로그 유지
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s -> %s | Attempts: %d/%d"),
        *Message,
        *Result,
        NBPlayerState->GetCurrentAttempts(),
        NBPlayerState->GetMaxAttempts()
    );

    // 승리 처리
    if (Result == TEXT("3S 0B"))
    {
        GameMode->HandleWin(this);
        return;
    }

    // 무승부 확인
    GameMode->CheckDraw();
}

void ANBPlayerController::ClientRPC_ShowGuessResult_Implementation(
    const FString& Guess,
    const FString& Result,
    int32 CurrentAttempts,
    int32 MaxAttempts)
{
    if (ChatInputWidget)
    {
        ChatInputWidget->ShowGuessResult(
            Guess,
            Result,
            CurrentAttempts,
            MaxAttempts
        );
    }
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
        if (Message == TEXT("NEW GAME STARTED!"))
        {
            ChatInputWidget->ClearResult();
        }

        ChatInputWidget->ShowGameMessage(Message);
    }
}
