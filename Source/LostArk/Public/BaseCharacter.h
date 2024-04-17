// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" 
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "BaseCharacter.generated.h"



class AWeapon;  // ����
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter //,public IHitInterface ��Ŭ��� �ϰ��� ����
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	// ���̾ư���
	//UPROPERTY(EditAnywhere)
	//class UNiagaraSystem* NI_Bossskill1;

	//USceneComponent* NiagaraSceneComp;
	//class UNiagaraComponent* NiagaraComp;

	


	

protected:
	virtual void BeginPlay() override;

	virtual void Attack();  //����
	virtual void Die(); // ����
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();  // ���� ������ ������ �������� üũ
	bool IsAlive();
	void PlayHitReactMontage(const FName& SectionName); // ��Ʈ����Ʈ����
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName);  // �ݺ����� �ʰ� ��Ÿ�� �̴� �迭


	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();   // ����������

	/* ����
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled); //  */
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes; // hp

	
	/*����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	*/

	/**
	*  �ִϸ��̼� ����
	*/

	// ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage; 

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;


	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	// ���̾ư��� �迭
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<UNiagaraSystem*> NiagaraSections;

	/**
	 * Components
	 */


	



private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	
	

	UPROPERTY(EditAnywhere, Category = Sounds) // ����
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects) // ��ƼŬ
	UParticleSystem* HitParticles;



};
