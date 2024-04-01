// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/BoxComponent.h" //박스컴포넌트
//#include "Items/Weapons/Weapon.h" 무기





ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack() // 공격
{

}

void ABaseCharacter::Die() // 죽음
{

}





void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint) // 지향성 타격 반응
{

}

void ABaseCharacter::PlayAttackMontage()  // 공격모션
{

}

// 데미지 받았을때 몽타주
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
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
/* 무기
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ignoreActors.Empty();
	}



}

*/

