// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/BoxComponent.h" //�ڽ�������Ʈ
//#include "Items/Weapons/Weapon.h" ����





ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack() // ����
{

}

void ABaseCharacter::PlayAttackMontage()  // ���ݸ��
{

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/* ����
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ignoreActors.Empty();
	}



}

*/

