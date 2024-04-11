// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h> // �ִϸ��̼� �ν��Ͻ�
#include "SlashCharacter.h" // ������ĳ����
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� �̵�	
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	

	

	// C++���� ������ ���� ĳ��Ʈ�� ����ϴ� ���
	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner()); // ��ȣ�ȿ� �ִ°� ������ ĳ���Ͱ� �ƴϸ� null�� ��ȯ��, �����Ϸ��� �׸��� null �̿��� null�� ��ȯ��
	if (SlashCharacter)
	{
		SlashCharacterMovement = SlashCharacter->GetCharacterMovement();
	}




}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SlashCharacterMovement)  // ĳ���� �̵����� Ȯ��
	{
		 
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharacterMovement->Velocity);  // ���͸� �Է����� ����,  �̵��ӵ�
		IsFalling = SlashCharacterMovement->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState(); // ĳ������ ������¸� �ҷ���

	}
}
