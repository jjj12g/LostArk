// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" 
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "BaseCharacter.generated.h"



class AWeapon;  // 무기
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter //,public IHitInterface 인클루드 하고나서 생성
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	// 나이아가라
	//UPROPERTY(EditAnywhere)
	//class UNiagaraSystem* NI_Bossskill1;

	//USceneComponent* NiagaraSceneComp;
	//class UNiagaraComponent* NiagaraComp;

	


	

protected:
	virtual void BeginPlay() override;

	virtual void Attack();  //공격
	virtual void Die(); // 죽음
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	virtual bool CanAttack();  // 지금 공격이 가능한 상태인지 체크
	bool IsAlive();
	void PlayHitReactMontage(const FName& SectionName); // 히트리액트동작
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName);  // 반복되지 않게 몽타주 뽑는 배열


	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();   // 마무리공격

	/* 무기
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled); //  */
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes; // hp

	
	/*무기
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	*/

	/**
	*  애니메이션 동작
	*/

	// 공격 동작
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

	// 나이아가라 배열
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<UNiagaraSystem*> NiagaraSections;

	/**
	 * Components
	 */


	



private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	
	

	UPROPERTY(EditAnywhere, Category = Sounds) // 사운드
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects) // 파티클
	UParticleSystem* HitParticles;



};
