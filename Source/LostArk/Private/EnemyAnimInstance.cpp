// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enemy = Cast<AEnemy>(GetOwningActor());   // 전역 변수화

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (enemy != nullptr)
	{
		currentState = enemy->EnemyState;

	}

}

// 돌진 공격 노티파이
void UEnemyAnimInstance::AnimNotify_rushAttack1()
{
	UE_LOG(LogTemp, Warning, TEXT("Notify"));
	enemy->rush = true;
	
}

void UEnemyAnimInstance::AnimNotify_rushAttack2()
{
	UE_LOG(LogTemp, Warning, TEXT("Notify2"));
	enemy->rush1 = true;
	enemy->EnemyoverlapOn = true;
}

void UEnemyAnimInstance::AnimNotify_rushAttack3()
{
	UE_LOG(LogTemp, Warning, TEXT("Notify3"));
	enemy->rush2 = true;

}

void UEnemyAnimInstance::AnimNotify_breath1()
{
	UE_LOG(LogTemp, Warning, TEXT("breath1"));
	enemy->breath1 = true;
	

}

void UEnemyAnimInstance::AnimNotify_OverlapOn()
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapOn"));
	enemy->EnemyoverlapOn = true;
}

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd"));
	enemy->AttackEnd();
	enemy->EnemyoverlapOn = false;
	enemy->dontMove = false;
	enemy->look = true;
}

void UEnemyAnimInstance::AnimNotify_rain()
{
	UE_LOG(LogTemp, Warning, TEXT("rain11111111"));

}

void UEnemyAnimInstance::AnimNotify_rain1()
{
	UE_LOG(LogTemp, Warning, TEXT("rain222222222222"));

}







