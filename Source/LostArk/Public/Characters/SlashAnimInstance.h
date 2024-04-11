// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h" // ������ĳ������ ���¸� �о���� ����
#include "SlashAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; // ����÷��� �Լ��� ���
	virtual void NativeUpdateAnimation(float DeltaTime) override;       // ����Ƽ�� ������Ʈ �ִϸ��̼�, ƽ�Լ��� ���




	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* SlashCharacterMovement;   // ĳ���� ���� ���� ���

	UPROPERTY(BlueprintReadOnly, Category = Movement) // ĳ���� �̵��ӵ�
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;

};
