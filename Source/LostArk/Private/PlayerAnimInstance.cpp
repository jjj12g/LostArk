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

void UPlayerAnimInstance::AnimNotify_nnHit1() //땅 내리찍기 HitGroundMontage
{
	player->ShootBullet2();
}

void UPlayerAnimInstance::AnimNotify_nnSky1() //하늘에서 기모으기 SkyMontage
{
	player->ShootBullet5();
}
 
void UPlayerAnimInstance::AnimNotify_nnShoot1() //평타 StraightForwardMontage
{

}

void UPlayerAnimInstance::AnimNotify_nnPierce1() //찌르기 FencingMontage
{

}
