#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBChatInput.generated.h"

class UTextBlock;
class UEditableTextBox;

UCLASS()
class NUMBERBASEBALL_API UNBChatInput : public UUserWidget
{
    GENERATED_BODY()

public:
    // 숫자 야구 판정 결과를 각 UI에 나누어 표시
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ShowGuessResult(
        const FString& Guess,
        const FString& Result,
        int32 CurrentAttempts,
        int32 MaxAttempts
    );

    // 잘못된 입력 등의 안내 메시지
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ShowResult(const FString& Message);

    // 승리 / 무승부 / 새 게임 메시지
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ShowGameMessage(const FString& Message);

    // 새 게임 시작 시 결과 초기화
    UFUNCTION(BlueprintCallable, Category = "Number Baseball")
    void ClearResult();

    // 입력창 포커스
    void FocusChatInput();

protected:
    // 기존 숫자 입력창
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

    // 기존 결과 텍스트: 안내 메시지용으로 유지
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_Result;

    // 게임 종료 / 시작 메시지
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_GameMessage;

    // 마지막 입력 숫자
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_LastGuess;

    // 스트라이크 / 볼 판정
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_JudgeResult;

    // 시도 횟수
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TextBlock_Attempts;
};
