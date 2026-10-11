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

    // 입력한 숫자를 서버로 전송
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Chat")
    void ServerRPC_SendChatMessage(const FString& Message);

    // 서버의 판정 결과를 해당 플레이어에게 전달
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowResult(const FString& Message);

    // 서버의 게임 종료 안내를 모든 플레이어에게 전달하기 위한 함수
    UFUNCTION(Client, Reliable)
    void ClientRPC_ShowGameMessage(const FString& Message);

protected:
    // 에디터에서 지정할 입력 UI 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UNBChatInput> ChatInputWidgetClass;

    // 생성된 입력 UI
    UPROPERTY()
    TObjectPtr<UNBChatInput> ChatInputWidget;
};
