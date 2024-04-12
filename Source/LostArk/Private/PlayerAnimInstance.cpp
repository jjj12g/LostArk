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
	// SlashCharacter의 bool 변수인 bPlayerIsAttacking을 false로 설정한다 (=이동 입력을 받을 수 있도록 초기화)
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