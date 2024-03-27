// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


 //class AWeapon;   ����



UCLASS()
class LOSTARK_API ABaseCharacter : public ACharacter
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


	virtual void PlayAttackMontage();

	virtual bool CanAttack();  // ���� ������ ������ �������� üũ

	UFUNCTION(BlueprintCallable)  
	virtual void AttackEnd();   // ����������

	
	/*����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;
	*/

	/**
	*  �ִϸ��̼� ��� �÷���
	*/

	// ���� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage; 


};
