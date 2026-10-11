#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UNBChatInput;

UCLASS()
class NUMBERBASEBALL_API ANBPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    // 클라이언트 입력을 서버로 전송
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Chat")
    void ServerRPC_SendChatMessage(const FString& Message);

    // 정상 입력 판정 결과를 해당 클라이언트에 전송
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowGuessResult(
        const FString& Guess,
        const FString& Result,
        int32 CurrentAttempts,
        int32 MaxAttempts
    );

    // 잘못된 입력 등의 안내
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowResult(const FString& Message);

    // 게임 종료 / 시작 메시지
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowGameMessage(const FString& Message);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UNBChatInput> ChatInputWidgetClass;

    UPROPERTY()
    TObjectPtr<UNBChatInput> ChatInputWidget;
};
