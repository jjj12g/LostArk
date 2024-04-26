// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "enemy/Enemy.h"
#include "textnibox.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	class AEnemy* enemy;  // ó������ ���ʹ̸� �޾Ƽ� ������ �о��

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	//class ASlashCharacter* player;  // ó������ ���ʹ̸� �޾Ƽ� ������ �о��


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	EEnemyState currentState;  // ���ʹ� ������Ʈ�� �޾ƿ�


	virtual void NativeInitializeAnimation() override;   // ����÷��̿� �ش�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;  // ƽ�� �ش�

	/*UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Bossskill1;

	USceneComponent* NiagaraSceneComp;
	class UNiagaraComponent* NiagaraComp;*/


	private:
	FTimerHandle deathTimer;

	UFUNCTION()                      
	void AnimNotify_rushAttack1();	 //  AnimNotify��� ���λ縦 ���̰� �ڿ��̸� (��ҹ��� �������)

	UFUNCTION()                      
	void AnimNotify_rushAttack2();

	UFUNCTION()                      
	void AnimNotify_rushAttack3();

	UFUNCTION()
	void AnimNotify_breath1();

	UFUNCTION()
	void  AnimNotify_breathAttack();

	UFUNCTION()
	void AnimNotify_OverlapOn();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_rain();

	UFUNCTION()
	void AnimNotify_rain1();

	UFUNCTION()
	void AnimNotify_rotator(); 

	UFUNCTION()
	void AnimNotify_pizza(); 

	UFUNCTION()
	void AnimNotify_tanaedo(); 

	UFUNCTION()
	void AnimNotify_pizzaStart();

	UFUNCTION()
	void AnimNotify_pizzaOn1();

	UFUNCTION()
	void AnimNotify_pizzaOn2();

	UFUNCTION()
	void AnimNotify_pizzaOn3();

	UFUNCTION()
	void AnimNotify_pizzaOn4();

};
