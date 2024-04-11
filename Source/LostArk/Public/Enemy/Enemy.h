// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //���ʹ̿� �÷��̾��� ����θ�
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // ĳ���� �������� �ִϸ��̼� ���۳����� ����Ұ�.
#include "Enemy.generated.h"




class UHealthBarComponent; // hp ��
class UPawnSensingComponent; //�� �������� ���漱��






UCLASS()
class LOSTARK_API AEnemy : public ABaseCharacter, public IHitInterface
{
	GENERATED_BODY()


public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigetor, AActor* DamageCauser) override; // �������ý���
	//virtual void Destroyed() override;
	/** </AActor> */


	/** <IHitInterface>  */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/** </IHitInterface>  */

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;  // ����
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	/** </ABaseCharacter> */

	// ���� TAKE ������
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	)override;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte <EDeathPose> DeathPose; // ���� ���

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //�ִϸ��̼� ��������

	//void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere)
	float HP = 100;

private:
	/** AI behavior(�ൿ) */
	void InitializeEnemy(); // ����÷��̿��� ������ �ʱ�ȭ�Լ�
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished(); // ���� Ÿ�̸Ӱ� ������ ����
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();  // ��������
	void ChaseTarget();
	bool IsOutsideCombatRadius(); // ���ݹ����� �ƹ������ ����
	bool IsOutsideAttackRadius(); // ���ݹ��� �� Ÿ�� �߰�
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer(); // �����ð�
	void StartAttackTimer(); /** Combat */
	void ClearAttackTimer(); /** Combat */
	bool InTargetRange(AActor* Target, double Radius); // Ÿ���� �������� �ִٸ� true�� ��ȯ     ����ݰ�
	void MoveToTarget(AActor* Target);   //�����ð������� �̵�
	AActor* ChoosePatrolTarget(); //�� ǥ�� �����ϱ�
	

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp ��

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // ���� ��ġ����(�ð�, û��) ������Ʈ ����

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 300.f;   // ���� �ݰ�

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
	
	// �ּҼ����ð� �ִ�����ð� ����
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float patrollingSpeed = 125.f;
	
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	// ���� Ÿ�ٹ߰��������� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.0f;
};