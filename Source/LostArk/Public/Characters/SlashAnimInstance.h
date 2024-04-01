// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h" // 슬래쉬캐릭터의 상태를 읽어오기 위함
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; // 비긴플레이 함수와 비슷
	virtual void NativeUpdateAnimation(float DeltaTime) override;       // 네이티브 업데이트 애니메이션, 틱함수와 비슷




	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;   // 캐릭터 동작 구성 요소

	UPROPERTY(BlueprintReadOnly, Category = Movement) // 캐릭터 이동속도
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

};
