
#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"

ANBPlayerState::ANBPlayerState()
{
    // PlayerState는 기본적으로 복제되지만 명시적으로 설정
    bReplicates = true;
}

int32 ANBPlayerState::GetCurrentAttempts() const
{
    return CurrentAttempts;
}

int32 ANBPlayerState::GetMaxAttempts() const
{
    return MaxAttempts;
}

bool ANBPlayerState::CanAttempt() const
{
    return CurrentAttempts < MaxAttempts;
}

void ANBPlayerState::AddAttempt()
{
    if (HasAuthority() && CanAttempt())
    {
        ++CurrentAttempts;
    }
}

void ANBPlayerState::ResetAttempts()
{
    if (HasAuthority())
    {
        CurrentAttempts = 0;
    }
}

void ANBPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ANBPlayerState, CurrentAttempts);
}
