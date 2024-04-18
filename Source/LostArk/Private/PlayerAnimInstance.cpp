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
	 
	 
}

void UPlayerAnimInstance::AnimNotify_nnIdle()
{
	// SlashCharacter�� bool ������ bPlayerIsAttacking�� false�� �����Ѵ� (=�̵� �Է��� ���� �� �ֵ��� �ʱ�ȭ)
	UE_LOG(LogTemp, Warning, (TEXT("nnIdle")));
	player->bPlayerIsAttacking = false;
	player->bKeyPressed = false;
	player->bAttackEnabled = false;
	if(!player->bKeyPressed)
	{
		UE_LOG(LogTemp, Warning, (TEXT("bKeyPressed FALSE")));
	}
}

void UPlayerAnimInstance::AnimNotify_nnHit1()
{
	player->ShootBullet2();
}

void UPlayerAnimInstance::AnimNotify_nnSky1()
{
	player->ShootBullet5();
}

//void UPlayerAnimInstance::AnimNotify_nnPierce1()
//{
//	player->ShootBullet();
//}

void UPlayerAnimInstance::AnimNotify_nnBasic()
{
	player->ShootBullet();
}
