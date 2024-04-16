//Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	/** <IHitInterface>  */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/** </IHitInterface>  */

	
	// ���̾ư���
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Bossskill1;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* Lighting;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_breath;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* Dash;



	USceneComponent* NiagaraSceneComp;

	class UNiagaraComponent* NiagaraComp;
	
	class ASlashCharacter* player;

	class UMaterialInstanceDynamic* dynamicMAT;

	class UMaterialInstanceDynamic* dynamicMAT1;

	

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //�ִϸ��̼� ��������

	// ���� ��Ƽ���� �ý���
	bool rushAttack(float deltaSeconds);
	bool rush = false;
	bool rush1 = false;
	bool rush2 = false;
	bool breath1 = false;
	bool EnemyoverlapOn = false;
	bool EnemyoveralpOff = false;

	


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

	// ������ �̺�Ʈ
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mysettings")
	class AEnemy* enemy;

	// ���� ��� �Լ�
	void AttackMontage1();
	void AttackMontage2();
	void AttackMontage3();
	void AttackMontage4();
	void AttackMontage5();
	void AttackMontage6();
	void AttackMontage7();
	void AttackMontage8();
	void AttackMontage9();
	void AttackMontage10();

	void enemyCollisionOn();
	void enemyCollisionOff();

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;

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

	float stackTime = 0.0f;

	//�̵��ӵ�
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 600.f;   // ���� �ݰ�

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
	
	FVector targetLoc;
	FVector startLoc;
	

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	// ���� Ÿ�ٹ߰��������� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.0f;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class EnemyAnimInstance* Anim;*/

	//UFUNCTION()
	//void AnimNotify_rushAttack1();
};