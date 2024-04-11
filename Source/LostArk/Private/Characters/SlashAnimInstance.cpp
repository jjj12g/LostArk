// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h> // 애니메이션 인스턴스
#include "SlashCharacter.h" // 슬래쉬캐릭터
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 이동	
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	

	

	// C++에서 엔진을 위해 캐스트를 사용하는 방법
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner()); // 괄호안에 있는게 슬래쉬 캐릭터가 아니면 null을 반환함, 전달하려는 항목이 null 이여도 null을 반환함
	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}




}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCharacterMovement)  // 캐릭터 이동여부 확인
	{
		 
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);  // 백터를 입력으로 가짐,  이동속도
		IsFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState(); // 캐릭터의 현재상태를 불러옴

	}
}
