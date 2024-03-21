// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class LOSTARK_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;






	
private:


	// 대상
	UPROPERTY()
	AActor* CombatTarget;

	//이동속도
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	/** 
	*Navigation 
	*/

	UPROPERTY()
	class AAIController* EnemyController;  //액터가 지시사항 전달 컨트롤러에 변수설정

	// 현재 순찰 대상, 배열로 갖게될것임
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;  

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;










protected:
	virtual void BeginPlay() override;

	// 타깃이 범위내에 있다면 true값 반환     수용반경
	bool InTargetRange(AActor* Target, double Radius);






public:	
	
	


};
