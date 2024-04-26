// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enemy = Cast<AEnemy>(GetOwningActor());   // 전역 변수화
	
	//player = Cast<ASlashCharacter>(GetOwningActor());

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

void UEnemyAnimInstance::AnimNotify_breathAttack()
{
	
	UE_LOG(LogTemp, Warning, TEXT("breathAttackDamage"));
	enemy->breathDamage = true;
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

void UEnemyAnimInstance::AnimNotify_rotator()
{

	enemy->dontMove = false;
	enemy->bRotator = true;

}

void UEnemyAnimInstance::AnimNotify_pizza()
{
	enemy->bPizza = true;

}

void UEnemyAnimInstance::AnimNotify_tanaedo()
{
	enemy->btonado = true;
	UE_LOG(LogTemp,Warning,TEXT("tanaedo"));


}

void UEnemyAnimInstance::AnimNotify_pizzaStart()
{
	enemy->bPizzaSound = true;

}

void UEnemyAnimInstance::AnimNotify_pizzaOn1()
{
	enemy->bPizzaNI1 = true;
}

void UEnemyAnimInstance::AnimNotify_pizzaOn2()
{
	enemy->bPizzaNI2 = true;
}

void UEnemyAnimInstance::AnimNotify_pizzaOn3()
{
	enemy->bPizzaNI3 = true;
}

void UEnemyAnimInstance::AnimNotify_pizzaOn4()
{
	enemy->bPizzaNI4 = true;
}










