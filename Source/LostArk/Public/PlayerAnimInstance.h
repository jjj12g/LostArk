// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashCharacter.h"
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

	
	class ASlashCharacter* player;

private:
	

	UFUNCTION()
	void AnimNotify_nnIdle();

UFUNCTION()
	void AnimNotify_nnHit1(); //�� ������� HitGroundMontage

	UFUNCTION()
	void AnimNotify_nnSky1(); //�ϴÿ��� ������� SkyMontage

	UFUNCTION()
	void AnimNotify_nnShoot1(); //��Ÿ StraightForwardMontage

	UFUNCTION()
	void AnimNotify_nnPierce1(); //��� FencingMontage


	
	
};
