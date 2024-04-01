// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" 
#include "BaseCharacter.generated.h"


 //class AWeapon;   ����



UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter //,public IHitInterface ��Ŭ��� �ϰ��� ����
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;


	/* ����
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled); //  */

protected:
	virtual void BeginPlay() override;
	virtual void Attack();  //����
	virtual void Die(); // ����

	/**
	* �÷��� ���� ���
	*/

	
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName); // ��Ʈ����Ʈ����
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual bool CanAttack();  // ���� ������ ������ �������� üũ

	UFUNCTION(BlueprintCallable)  
	virtual void AttackEnd();   // ����������

	
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

};
