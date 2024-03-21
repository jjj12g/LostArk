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


	// ���
	UPROPERTY()
	AActor* CombatTarget;

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	/** 
	*Navigation 
	*/

	UPROPERTY()
	class AAIController* EnemyController;  //���Ͱ� ���û��� ���� ��Ʈ�ѷ��� ��������

	// ���� ���� ���, �迭�� ���Եɰ���
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;  

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;










protected:
	virtual void BeginPlay() override;

	// Ÿ���� �������� �ִٸ� true�� ��ȯ     ����ݰ�
	bool InTargetRange(AActor* Target, double Radius);






public:	
	
	


};
