#include "NBChatInput.h"
#include "Components/TextBlock.h"

void UNBChatInput::ShowResult(const FString& Message)
{
    if (TextBlock_Result)
    {
        TextBlock_Result->SetText(FText::FromString(Message));
    }
}

void UNBChatInput::ShowGameMessage(const FString& Message)
{
    if (TextBlock_GameMessage)
    {
        TextBlock_GameMessage->SetText(FText::FromString(Message));
    }
}
