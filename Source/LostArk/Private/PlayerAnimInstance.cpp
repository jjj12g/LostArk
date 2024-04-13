// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "SlashCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	player = Cast<ASlashCharacter>(GetOwningActor());

	if (player != nullptr)
	{
		playerMovementComp = player->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	 Super::NativeUpdateAnimation(DeltaSeconds);
	 
	 if (playerMovementComp != nullptr)
	 {
		
	 }
}

void UPlayerAnimInstance::AnimNotify_nnIdle()
{
	// SlashCharacter�� bool ������ bPlayerIsAttacking�� false�� �����Ѵ� (=�̵� �Է��� ���� �� �ֵ��� �ʱ�ȭ)
	player->bPlayerIsAttacking=false;
}

void UPlayerAnimInstance::AnimNotify_nnHit()
{
	player->ShootBullet2();
}

void UPlayerAnimInstance::AnimNotify_nnHit2()
{
	player->ShootBullet5();
}