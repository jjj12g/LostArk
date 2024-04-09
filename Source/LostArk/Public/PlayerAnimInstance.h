// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UCharacterMovementComponent* playerMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bRunMotionOn = false;

private:
	UPROPERTY()
	class ASlashCharacter* player;

	
	
};
