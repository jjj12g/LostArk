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
	class AEnemy* enemy;  // 처음부터 에너미를 받아서 변수를 읽어옴

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	//class ASlashCharacter* player;  // 처음부터 에너미를 받아서 변수를 읽어옴


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings")
	EEnemyState currentState;  // 에너미 스테이트를 받아옴


	virtual void NativeInitializeAnimation() override;   // 비긴플레이에 해당
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;  // 틱에 해당

	/*UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Bossskill1;

	USceneComponent* NiagaraSceneComp;
	class UNiagaraComponent* NiagaraComp;*/


	private:
	FTimerHandle deathTimer;

	UFUNCTION()                      
	void AnimNotify_rushAttack1();	 //  AnimNotify라는 접두사를 붙이고 뒤에이름 (대소문자 맞춰야함)

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
