
#include "NBChatInput.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

// 마지막 입력 숫자, 판정 결과, 시도 횟수 표시
void UNBChatInput::ShowGuessResult(
    const FString& Guess,
    const FString& Result,
    int32 CurrentAttempts,
    int32 MaxAttempts)
{
    // 마지막 입력 숫자
    if (TextBlock_LastGuess)
    {
        TextBlock_LastGuess->SetText(
            FText::FromString(Guess)
        );
    }

    // 스트라이크 / 볼 판정
    if (TextBlock_JudgeResult)
    {
        TextBlock_JudgeResult->SetText(
            FText::FromString(Result)
        );
    }

    // 시도 횟수
    if (TextBlock_Attempts)
    {
        const FString AttemptsText = FString::Printf(
            TEXT("ATTEMPTS %d / %d"),
            CurrentAttempts,
            MaxAttempts
        );

        TextBlock_Attempts->SetText(
            FText::FromString(AttemptsText)
        );
    }

    // 이전 오류 안내 메시지 제거
    if (TextBlock_Result)
    {
        TextBlock_Result->SetText(FText::GetEmpty());
    }

    // 첫 정상 입력 시 새 게임 안내 메시지 제거
    if (TextBlock_GameMessage)
    {
        const FString GameMessage =
            TextBlock_GameMessage->GetText().ToString();

        if (GameMessage.Contains(TEXT("NEW GAME STARTED")))
        {
            TextBlock_GameMessage->SetText(FText::GetEmpty());
        }
    }
}

// 잘못된 입력 등의 안내 메시지 표시
void UNBChatInput::ShowResult(const FString& Message)
{
    // 잘못된 입력 시에도 새 게임 안내 메시지 제거
    if (TextBlock_GameMessage)
    {
        const FString GameMessage =
            TextBlock_GameMessage->GetText().ToString();

        if (GameMessage.Contains(TEXT("NEW GAME STARTED")))
        {
            TextBlock_GameMessage->SetText(FText::GetEmpty());
        }
    }

    // 오류 안내 메시지 표시
    if (TextBlock_Result)
    {
        TextBlock_Result->SetText(
            FText::FromString(Message)
        );
    }
}

// 승리 / 무승부 / 새 게임 메시지 표시 및 색상 변경
void UNBChatInput::ShowGameMessage(const FString& Message)
{
    if (!TextBlock_GameMessage)
    {
        return;
    }

    // 게임 상태 메시지 표시
    TextBlock_GameMessage->SetText(
        FText::FromString(Message)
    );

    FLinearColor MessageColor = FLinearColor::White;

    if (Message.Contains(TEXT("WINNER")))
    {
        // 승리: 골드
        MessageColor = FLinearColor(
            0.953f, 0.788f, 0.420f, 1.0f
        );
    }
    else if (Message.Contains(TEXT("DRAW")))
    {
        // 무승부: 레드
        MessageColor = FLinearColor(
            0.898f, 0.545f, 0.545f, 1.0f
        );
    }
    else if (Message.Contains(TEXT("NEW GAME STARTED")))
    {
        // 새 게임: 라이트 블루
        MessageColor = FLinearColor(
            0.561f, 0.780f, 0.910f, 1.0f
        );
    }

    // 메시지 색상 적용
    TextBlock_GameMessage->SetColorAndOpacity(
        FSlateColor(MessageColor)
    );
}

// 새 게임 시작 시 UI 초기화
void UNBChatInput::ClearResult()
{
    // 마지막 입력 숫자 초기화
    if (TextBlock_LastGuess)
    {
        TextBlock_LastGuess->SetText(
            FText::FromString(TEXT("---"))
        );
    }

    // 판정 결과 초기화
    if (TextBlock_JudgeResult)
    {
        TextBlock_JudgeResult->SetText(
            FText::FromString(TEXT("---"))
        );
    }

    // 시도 횟수 초기화
    if (TextBlock_Attempts)
    {
        TextBlock_Attempts->SetText(
            FText::FromString(TEXT("ATTEMPTS 0 / 3"))
        );
    }

    // 오류 안내 메시지 제거
    if (TextBlock_Result)
    {
        TextBlock_Result->SetText(FText::GetEmpty());
    }

    // 입력창 초기화
    if (EditableTextBox_ChatInput)
    {
        EditableTextBox_ChatInput->SetText(FText::GetEmpty());
    }
}

// 입력창에 키보드 포커스 설정
void UNBChatInput::FocusChatInput()
{
    if (EditableTextBox_ChatInput)
    {
        EditableTextBox_ChatInput->SetKeyboardFocus();
    }
}
