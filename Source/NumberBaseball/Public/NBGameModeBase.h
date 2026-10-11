#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

class ANBPlayerController;

UCLASS()
class NUMBERBASEBALL_API ANBGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANBGameModeBase();

    virtual void BeginPlay() override;

    bool IsValidGuess(const FString& Guess) const;
    FString JudgeGuess(const FString& Guess) const;

    bool IsGameFinished() const;

    void HandleWin(ANBPlayerController* Winner);
    void CheckDraw();

protected:
    FString SecretNumber;
    bool bGameFinished = false;

    FTimerHandle RestartTimerHandle;

    void GenerateSecretNumber();
    void RestartNumberBaseball();

    // 모든 플레이어에게 게임 상태 안내
    void BroadcastGameMessage(const FString& Message);
};
