//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //에너미와 플레이어의 공통부모
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // 캐릭터 죽음같은 애니메이션 동작넣을때 사용할것.
#include "Enemy.generated.h"




class UHealthBarComponent; // hp 바
class UPawnSensingComponent; //폰 감지센서 전방선언






UCLASS()
class LOSTARK_API AEnemy : public ABaseCharacter, public IHitInterface
{
	GENERATED_BODY()


public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigetor, AActor* DamageCauser) override; // 데미지시스템
	//virtual void Destroyed() override;
	/** </AActor> */

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	/** <IHitInterface>  */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/** </IHitInterface>  */

	
	// 나이아가라
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
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //애니메이션 순찰에들어감

	// 공격 노티파이 시스템
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
	virtual void Attack() override;  // 공격
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	/** </ABaseCharacter> */

	// 오버랩 이벤트
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mysettings")
	class AEnemy* enemy;

	// 공격 모션 함수
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

	// 요한 TAKE 데미지
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	)override;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte <EDeathPose> DeathPose; // 죽음 모션

	

	//void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere)
	float HP = 100;

private:
	/** AI behavior(행동) */
	void InitializeEnemy(); // 비긴플레이에서 실행할 초기화함수
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished(); // 순찰 타이머가 끝나면 실행
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();  // 순찰시작
	void ChaseTarget();
	bool IsOutsideCombatRadius(); // 공격범위에 아무도없어서 순찰
	bool IsOutsideAttackRadius(); // 공격범위 밖 타겟 추격
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer(); // 순찰시간
	void StartAttackTimer(); /** Combat */
	void ClearAttackTimer(); /** Combat */
	bool InTargetRange(AActor* Target, double Radius); // 타깃이 범위내에 있다면 true값 반환     수용반경
	void MoveToTarget(AActor* Target);   //순찰시간끝나면 이동
	AActor* ChoosePatrolTarget(); //새 표적 선택하기


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp 바

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // 폰의 위치감지(시각, 청각) 컴포넌트 생성

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	AActor* CombatTarget;

	float stackTime = 0.0f;

	//이동속도
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 600.f;   // 공격 반경

	UPROPERTY()
	class AAIController* EnemyController;  //액터가 지시사항 전달 컨트롤러에 변수설정

	// 현재 순찰 대상, 배열로 갖게될것임
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// 전투반경
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;  // 순찰 타이머
	
	// 최소순찰시간 최대순찰시간 배정
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	// 순찰 속도
	UPROPERTY(EditAnywhere, Category = "Combat")
	float patrollingSpeed = 125.f;
	
	FTimerHandle AttackTimer;
	
	FVector targetLoc;
	FVector startLoc;
	

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	// 순찰 타겟발견했을때의 속도
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.0f;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class EnemyAnimInstance* Anim;*/

	//UFUNCTION()
	//void AnimNotify_rushAttack1();
};