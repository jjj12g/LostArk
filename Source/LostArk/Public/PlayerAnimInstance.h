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
	void AnimNotify_nnHit1(); //땅 내리찍기 HitGroundMontage

	UFUNCTION()
	void AnimNotify_nnSky1(); //하늘에서 기모으기 SkyMontage

	//UFUNCTION()
	//void AnimNotify_nnShoot1(); //평타 StraightForwardMontage

	//UFUNCTION()
	//void AnimNotify_nnPierce1(); //찌르기 FencingMontage
	
	UFUNCTION()
	void AnimNotify_hit(); //하늘에서 기모으기 SkyMontage


	UFUNCTION()
	void AnimNotify_nnBasic(); //평타 기본공격
};
