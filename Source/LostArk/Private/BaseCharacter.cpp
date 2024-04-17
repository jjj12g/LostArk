// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/BoxComponent.h" //�ڽ�������Ʈ
#include "Items/Weapons/Weapon.h" //����
#include "Components/AttributeComponent.h"  // hp
#include "Components/CapsuleComponent.h" // ĸ�����۳�Ʈ
#include "NiagaraComponent.h" // ���̾ư��� ����Ʈ
#include "NiagaraFunctionLibrary.h" // ���̾ư���
#include "Kismet/GameplayStatics.h"
#include "CharacterTypes.h"





ABaseCharacter::ABaseCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); // ī�޶�� �浹 ����ȭ

}




void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack() // ����
{
	
}

void ABaseCharacter::Die() // ����
{

}





void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint) // ���⼺ Ÿ�� ����
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





int32 ABaseCharacter::PlayAttackMontage()  // ���ݸ��, ��Ÿ�ּ����� �� ��ȯ
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);

}

int32 ABaseCharacter::PlayDeathMontage() // �������
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}





void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // ���ʹ̰� �׾����� �ݸ����� ������� ��.
}

// ������ �޾����� ��Ÿ��
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

