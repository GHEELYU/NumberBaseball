#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBChatInput.generated.h"

class UTextBlock;

UCLASS()
class NUMBERBASEBALL_API UNBChatInput : public UUserWidget
{
    GENERATED_BODY()

public:
    // 판정 결과 출력
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ShowResult(const FString& Message);

    // 게임 종료 / 시작 안내 출력
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ShowGameMessage(const FString& Message);

protected:
    // 위젯 BP의 TextBlock 이름과 일치해야 함
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_Result;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_GameMessage;
};
