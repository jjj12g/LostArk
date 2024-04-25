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
	if (!player->bKeyPressed)
	{
		UE_LOG(LogTemp, Warning, (TEXT("bKeyPressed FALSE")));
	}
}

void UPlayerAnimInstance::AnimNotify_nnHit1() // ���� E
{
	player->ShootBullet2();
	UE_LOG(LogTemp, Warning, (TEXT("Thunder E")));
}

void UPlayerAnimInstance::AnimNotify_nnSky1() // ���׿� R
{
	player->ShootBullet5();
	UE_LOG(LogTemp, Warning, (TEXT("Meteor R")));
}

void UPlayerAnimInstance::AnimNotify_nnShoot1() // W
{
	player->ShootBullet4();
	UE_LOG(LogTemp, Warning, (TEXT("Niagara W")));
}

void UPlayerAnimInstance::AnimNotify_nnPierce1() // Q ��
{
	player->ShootBullet3();
	UE_LOG(LogTemp, Warning, (TEXT("Fire Q")));
}

//void UPlayerAnimInstance::AnimNotify_nnPierce1()
//{
//	player->ShootBullet();
//}

void UPlayerAnimInstance::AnimNotify_hit() // �ǰ� ��Ÿ��
{
	UE_LOG(LogTemp, Warning, (TEXT("bKeyPressed FALSE")));
}

void UPlayerAnimInstance::AnimNotify_nnBasic() // �⺻��Ÿ
{
	player->ShootBullet();
}
