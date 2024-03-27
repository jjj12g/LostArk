// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


 //class AWeapon;   무기



UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter
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


	virtual void PlayAttackMontage();

	virtual bool CanAttack();  // 지금 공격이 가능한 상태인지 체크

	UFUNCTION(BlueprintCallable)  
	virtual void AttackEnd();   // 마무리공격

	
	/*무기
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	*/

	/**
	*  애니메이션 펑션 플레이
	*/

	// 공격 몽타주
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage; 


};
