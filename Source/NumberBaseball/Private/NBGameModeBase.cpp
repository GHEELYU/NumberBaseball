#include "NBGameModeBase.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"

ANBGameModeBase::ANBGameModeBase()
{
    PlayerStateClass = ANBPlayerState::StaticClass();
}

void ANBGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    GenerateSecretNumber();
}

void ANBGameModeBase::GenerateSecretNumber()
{
    TArray<int32> Numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int32 i = Numbers.Num() - 1; i > 0; --i)
    {
        const int32 RandomIndex = FMath::RandRange(0, i);
        Numbers.Swap(i, RandomIndex);
    }

    SecretNumber = FString::Printf(
        TEXT("%d%d%d"),
        Numbers[0],
        Numbers[1],
        Numbers[2]
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Secret Number: %s"),
        *SecretNumber
    );
}

bool ANBGameModeBase::IsValidGuess(const FString& Guess) const
{
    if (Guess.Len() != 3)
    {
        return false;
    }

    for (int32 i = 0; i < 3; ++i)
    {
        if (Guess[i] < TEXT('1') || Guess[i] > TEXT('9'))
        {
            return false;
        }

        for (int32 j = i + 1; j < 3; ++j)
        {
            if (Guess[i] == Guess[j])
            {
                return false;
            }
        }
    }

    return true;
}

FString ANBGameModeBase::JudgeGuess(const FString& Guess) const
{
    int32 Strike = 0;
    int32 Ball = 0;

    for (int32 i = 0; i < 3; ++i)
    {
        if (Guess[i] == SecretNumber[i])
        {
            ++Strike;
        }
        else if (SecretNumber.Contains(FString::Chr(Guess[i])))
        {
            ++Ball;
        }
    }

    if (Strike == 0 && Ball == 0)
    {
        return TEXT("OUT");
    }

    return FString::Printf(
        TEXT("%dS %dB"),
        Strike,
        Ball
    );
}

bool ANBGameModeBase::IsGameFinished() const
{
    return bGameFinished;
}

void ANBGameModeBase::BroadcastGameMessage(const FString& Message)
{
    // 서버에 연결된 모든 PlayerController에 메시지 전송
    for (FConstPlayerControllerIterator It =
        GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        ANBPlayerController* PC =
            Cast<ANBPlayerController>(It->Get());

        if (PC)
        {
            PC->ClientRPC_ShowGameMessage(Message);
        }
    }
}

void ANBGameModeBase::HandleWin(ANBPlayerController* Winner)
{
    if (bGameFinished)
    {
        return;
    }

    bGameFinished = true;

    const FString WinnerName =
        Winner ? Winner->GetName() : TEXT("Unknown");

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("GAME OVER - WINNER: %s"),
        *WinnerName
    );

    BroadcastGameMessage(
        FString::Printf(TEXT("WINNER: %s"), *WinnerName)
    );

    GetWorldTimerManager().SetTimer(
        RestartTimerHandle,
        this,
        &ANBGameModeBase::RestartNumberBaseball,
        3.0f,
        false
    );
}

void ANBGameModeBase::CheckDraw()
{
    if (bGameFinished || !GameState)
    {
        return;
    }

    bool bHasPlayer = false;

    for (APlayerState* Player : GameState->PlayerArray)
    {
        ANBPlayerState* NBPlayer =
            Cast<ANBPlayerState>(Player);

        if (!NBPlayer)
        {
            continue;
        }

        bHasPlayer = true;

        if (NBPlayer->CanAttempt())
        {
            return;
        }
    }

    if (bHasPlayer)
    {
        bGameFinished = true;

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("GAME OVER - DRAW!")
        );

        BroadcastGameMessage(TEXT("GAME OVER - DRAW!"));

        GetWorldTimerManager().SetTimer(
            RestartTimerHandle,
            this,
            &ANBGameModeBase::RestartNumberBaseball,
            3.0f,
            false
        );
    }
}

void ANBGameModeBase::RestartNumberBaseball()
{
    bGameFinished = false;

    if (GameState)
    {
        for (APlayerState* Player : GameState->PlayerArray)
        {
            ANBPlayerState* NBPlayer =
                Cast<ANBPlayerState>(Player);

            if (NBPlayer)
            {
                NBPlayer->ResetAttempts();
            }
        }
    }

    GenerateSecretNumber();

    BroadcastGameMessage(TEXT("NEW GAME STARTED!"));
    UE_LOG(LogTemp, Warning, TEXT("NEW GAME STARTED!"));
}
