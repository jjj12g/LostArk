// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/BoxComponent.h" //박스컴포넌트
#include "Items/Weapons/Weapon.h" //무기
#include "Components/AttributeComponent.h"  // hp
#include "Components/CapsuleComponent.h" // 캡슐컴퍼넌트
#include "NiagaraComponent.h" // 나이아가라 이펙트
#include "NiagaraFunctionLibrary.h" // 나이아가라
#include "Kismet/GameplayStatics.h"
#include "CharacterTypes.h"





ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); // 카메라와 충돌 무력화

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

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}

}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);

	}

}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName)
{
	if (AttackMontageSections.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionName.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionName[Selection]);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSections[Selection], GetActorLocation(), GetActorRotation());

	return Selection;
}





int32 ABaseCharacter::PlayAttackMontage()  // 공격모션, 몽타주섹션의 값 반환
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);

}

int32 ABaseCharacter::PlayDeathMontage() // 죽음모션
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}





void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 에너미가 죽엇을때 콜리전을 사라지게 함.
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

bool ABaseCharacter::IsAlive()
{

	return Attributes && Attributes->IsAlive();

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

