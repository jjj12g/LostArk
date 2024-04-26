//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //���ʹ̿� �÷��̾��� ����θ�
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // ĳ���� �������� �ִϸ��̼� ���۳����� ����Ұ�.
#include "MybulletActor.h"
#include "tonaedoActor.h"
#include "boomActor.h"
#include "Enemy.generated.h"




//class UHealthBarComponent; // hp ��
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

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_SHOWKWAVE;


	// ����
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Pizee1;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Pizza2;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Pizza3;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Pizza4;



	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnLocation;

	// �Ҹ����� �ִ� ��
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AMybulletActor* abreath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraComponent* Dash;

	UPROPERTY(EditAnywhere, Category = "MySettings|Components")
	class UWidgetComponent* floatingWidgetComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class Atextnibox* testbullet;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AslowhitActor* hitbullet;

	USceneComponent* NiagaraSceneComp;

	class UNiagaraComponent* NiagaraComp;
	
	class ASlashCharacter* players;

	class UMaterialInstanceDynamic* dynamicMAT;

	class UMaterialInstanceDynamic* dynamicMAT1;
	
	//  ��ü ��ġ
	FVector e1; 
	FVector e2; 
	FVector e3;
	FVector e4; 
	FVector e5;
	FVector e6;
	FVector e7;
	FVector e8;
	FVector e9;
	FVector e10;

	bool bteste = false;
	void teste();

	// �Ҹ� ���� ����
	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<AMybulletActor> bullettospawn;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<AtonaedoActor> tanaedoActor;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<AboomActor> boomActor;

	AActor* ShootBullet();

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<Atextnibox> testspawn;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<AslowhitActor> testhitbullet;


	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //�ִϸ��̼� ��������

	// ���� ��Ƽ���� �ý���
	bool rushAttack(float deltaSeconds);
	bool rush = false;
	bool rush1 = false;
	bool rush2 = false;
	bool breath1 = false;
	bool EnemyoverlapOn = false;
	void MoveToTarget(AActor* Target);   //�����ð������� �̵�
	void AttackEnd();
	
	bool dontMove = false;
	bool look = false;
	bool bDie = false;
	// ���� �� ȸ��
	float rotTime = 0;
	FRotator rotStart;
	FRotator rotTarget;
	FVector loctarget;
	bool bLookTarget = false;
	bool btestTarget = false;
	bool breathDamage = false;
	// 3������
	bool threeAttack = false;
	bool threeAttacks = false;

	// ��������
	bool bRotator = false;
	bool bPizza = false;
	bool btonado = false;
	bool bPizzaSound = false;

	bool bPizzaNI1 = false;
	bool bPizzaNI2 = false;
	bool bPizzaNI3 = false;
	bool bPizzaNI4 = false;






	// ���ݹ���
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDistance = 600.0f;

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;  // ����
	virtual bool CanAttack() override;
	//virtual void HandleDamage(float DamageAmount) override;
	 void PlayDeathMontages();
	/** </ABaseCharacter> */

	// ������ �̺�Ʈ
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// ĳ���� ����
	//UFUNCTION()
	//FORCEINLINE AActor* GetcurrentTarget() {return target;};
	
	//UFUNCTION()
	//FORCEINLINE void removetarget() { target = nullptr; };

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

	void DeathMontage1();

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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 maxHP = 30;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* RushSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* braseSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* threeAttackSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* pizzeSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* tonaedoSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* vardathSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* oneAttackRotSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* basewolfSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
		class USoundBase* slowrainSound;

	UPROPERTY(EditAnywhere, Category ="MySettings")
	TSubclassOf<class UvidWidget> bp_vidWidget;

	UPROPERTY()
	class APlayerController* pc;

private:

	class UvidWidget* vidWidgetUI;

	/** AI behavior(�ൿ) */
	void InitializeEnemy(); // ����÷��̿��� ������ �ʱ�ȭ�Լ�
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished(); // ���� Ÿ�̸Ӱ� ������ ����
	//void HideHealthBar();
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
	
	AActor* ChoosePatrolTarget(); //�� ǥ�� �����ϱ�

	UPROPERTY()
	class AActor* target;

	FVector moveDir;   // �̵����� ������� ����
	
	//ü�°���
	int32 currentHP = 0;

	class UHealthBarWidget* EnemyWidget;

	FRotator BillboradwidgetComponent(class AActor* camActor);
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	/*UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;*/ //hp ��

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
	double AttackRadius = 1000.f;   // ���� �ݰ�

	UPROPERTY()
	class AAIController* EnemyController;  //���Ͱ� ���û��� ���� ��Ʈ�ѷ��� ��������

	// ���� ���� ���, �迭�� ���Եɰ���
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// �����ݰ�
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 3000.f;

	FTimerHandle PatrolTimer;  // ���� Ÿ�̸�
	
	// �ּҼ����ð� �ִ�����ð� ����
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float patrollingSpeed = 300.f;
	
	FTimerHandle AttackTimer;
	
	FVector targetLoc;
	FVector startLoc;
	

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	// ���� Ÿ�ٹ߰��������� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.0f;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class EnemyAnimInstance* Anim;*/

	//UFUNCTION()
	//void AnimNotify_rushAttack1();
};