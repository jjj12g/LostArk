// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //���ʹ̿� �÷��̾��� ����θ�
#include "CharacterTypes.h" // ĳ���� �������� �ִϸ��̼� ���۳����� ����Ұ�.
#include "Enemy.generated.h"


class UPawnSensingComponent; //�� �������� ���漱��





UCLASS()
class LOSTARK_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;






	
private:


	/*
	* Components
	*/


	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // ���� ��ġ����(�ð�, û��) ������Ʈ ����








	// ���
	UPROPERTY()
	AActor* CombatTarget;

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f; 

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;   // ���� �ݰ�

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

	// �����ݰ�
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;  // ���� Ÿ�̸�
	void PatrolTimerFinished(); // ���� Ÿ�̸Ӱ� ������ ����

	
	

	// �ּҼ����ð� �ִ�����ð� ����
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;


	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //�ִϸ��̼� ��������




protected:
	virtual void BeginPlay() override;

	// Ÿ���� �������� �ִٸ� true�� ��ȯ     ����ݰ�
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);   //�����ð������� �̵�
	AActor* ChoosePatrolTarget(); //�� ǥ�� �����ϱ�


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // ��������Ʈ





public:	
	
	


};
