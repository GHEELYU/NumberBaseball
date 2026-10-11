
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ANBPlayerState();

    // 플레이어별 최대 시도 횟수
    static constexpr int32 MaxAttempts = 3;

    // 현재 시도 횟수 가져오기
    UFUNCTION(BlueprintPure, Category = "Number Baseball")
    int32 GetCurrentAttempts() const;

    // 최대 시도 횟수 가져오기
    UFUNCTION(BlueprintPure, Category = "Number Baseball")
    int32 GetMaxAttempts() const;

    // 시도 횟수가 남아 있는지 확인
    bool CanAttempt() const;

    // 시도 횟수 1 증가
    void AddAttempt();

    // 시도 횟수 초기화
    void ResetAttempts();

protected:
    // 서버에서 변경한 값이 클라이언트에도 복제됨
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Number Baseball")
    int32 CurrentAttempts = 0;

    // 복제할 변수 등록
    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;
};
