// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" 
#include "BaseCharacter.generated.h"


 //class AWeapon;   무기



UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter //,public IHitInterface 인클루드 하고나서 생성
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;


	/* 무기
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled); //  */

protected:
	virtual void BeginPlay() override;
	virtual void Attack();  //공격
	virtual void Die(); // 죽음

	/**
	* 플레이 동작 펑션
	*/

	
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName); // 히트리액트동작
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual bool CanAttack();  // 지금 공격이 가능한 상태인지 체크

	UFUNCTION(BlueprintCallable)  
	virtual void AttackEnd();   // 마무리공격

	
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

};
