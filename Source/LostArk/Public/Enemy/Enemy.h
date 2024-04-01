// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //���ʹ̿� �÷��̾��� ����θ�
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // ĳ���� �������� �ִϸ��̼� ���۳����� ����Ұ�.
#include "Enemy.generated.h"


class UPawnSensingComponent; //�� �������� ���漱��
class UAttributeComponent; // hp
class UHealthBarComponent; // hp ��

class UAnimMontage;



UCLASS()
class LOSTARK_API AEnemy : public ABaseCharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	
	class UNiagaraSystem* NI_Bossskill1;
	USceneComponent* NiagaraSceneComp;
	class UNiagaraComponent* NiagaraComp;


	virtual void Tick(float DeltaTime) override;
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void GetHit(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigetor, AActor* DamageCauser) override; // �������ý���



	
private:
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes; // hp

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp ��

	/*
	* Components
	*/


	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // ���� ��ġ����(�ð�, û��) ������Ʈ ����a

	








	// ���
	UPROPERTY()
	AActor* CombatTarget;

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f; 

	UPROPERTY(EditAnywhere)
	double AttackRadius = 300.f;   // ���� �ݰ�

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

	
	



protected:
	virtual void BeginPlay() override;

	// Ÿ���� �������� �ִٸ� true�� ��ȯ     ����ݰ�
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);   //�����ð������� �̵�
	AActor* ChoosePatrolTarget(); //�� ǥ�� �����ϱ�
	virtual void Attack() override;  // ����
	virtual void PlayAttackMontage() override; // ���ݸ��
	


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // ��������Ʈ

	//UPROPERTY(BlueprintReadOnly)
	//EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //�ִϸ��̼� ��������

	



public:	
	
	


};
