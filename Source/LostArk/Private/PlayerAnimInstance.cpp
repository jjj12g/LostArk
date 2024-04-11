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
