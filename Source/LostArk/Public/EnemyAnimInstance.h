// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "enemy/Enemy.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	EEnemyState currentState;  // ó������ ���ʹ̸� �޾Ƽ� ������ �о��


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
	void AnimNotify_OverlapOn();

	UFUNCTION()
	void AnimNotify_AttackEnd();






};
