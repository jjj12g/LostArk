//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //에너미와 플레이어의 공통부모
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // 캐릭터 죽음같은 애니메이션 동작넣을때 사용할것.
#include "MybulletActor.h"
#include "tonaedoActor.h"
#include "boomActor.h"
#include "Enemy.generated.h"




//class UHealthBarComponent; // hp 바
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

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_SHOWKWAVE;


	// 피자
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

	// 불릿액터 넣는 곳
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
	
	//  구체 위치
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

	// 불릿 액터 관련
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
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //애니메이션 순찰에들어감

	// 공격 노티파이 시스템
	bool rushAttack(float deltaSeconds);
	bool rush = false;
	bool rush1 = false;
	bool rush2 = false;
	bool breath1 = false;
	bool EnemyoverlapOn = false;
	void MoveToTarget(AActor* Target);   //순찰시간끝나면 이동
	void AttackEnd();
	
	bool dontMove = false;
	bool look = false;
	bool bDie = false;
	// 공격 전 회전
	float rotTime = 0;
	FRotator rotStart;
	FRotator rotTarget;
	FVector loctarget;
	bool bLookTarget = false;
	bool btestTarget = false;
	bool breathDamage = false;
	// 3번공격
	bool threeAttack = false;
	bool threeAttacks = false;

	// 피자패턴
	bool bRotator = false;
	bool bPizza = false;
	bool btonado = false;
	bool bPizzaSound = false;

	bool bPizzaNI1 = false;
	bool bPizzaNI2 = false;
	bool bPizzaNI3 = false;
	bool bPizzaNI4 = false;
	bool bPizzadamage = false;

	



	// 공격범위
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float attackDistance = 600.0f;

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;  // 공격
	virtual bool CanAttack() override;
	//virtual void HandleDamage(float DamageAmount) override;
	 void PlayDeathMontages();
	/** </ABaseCharacter> */

	UPROPERTY(EditAnywhere)
	 class AboomActor* boomtarget;

	// 오버랩 이벤트
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 캐릭터 삭제
	//UFUNCTION()
	//FORCEINLINE AActor* GetcurrentTarget() {return target;};
	
	//UFUNCTION()
	//FORCEINLINE void removetarget() { target = nullptr; };

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

	void DeathMontage1();

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

	UPROPERTY(EditAnywhere, Category = "MySettings")
	int32 maxHP = 30000;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* RushSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* braseSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* threeAttackSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* pizzeSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* tonaedoSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* vardathSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* oneAttackRotSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* basewolfSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
		class USoundBase* slowrainSound;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UvidWidget> bp_vidWidget;

private:

	class UvidWidget* vidWidgetUI;
	/** AI behavior(행동) */
	void InitializeEnemy(); // 비긴플레이에서 실행할 초기화함수
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished(); // 순찰 타이머가 끝나면 실행
	//void HideHealthBar();
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
	
	AActor* ChoosePatrolTarget(); //새 표적 선택하기

	UPROPERTY()
	class AActor* target;

	FVector moveDir;   // 이동방향 잡기위한 변수
	
	//체력관련
	int32 currentHP = 0;

	class UHealthBarWidget* EnemyWidget;

	FRotator BillboradwidgetComponent(class AActor* camActor);
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	/*UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;*/ //hp 바

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
	double AttackRadius = 1000.f;   // 공격 반경

	UPROPERTY()
	class AAIController* EnemyController;  //액터가 지시사항 전달 컨트롤러에 변수설정

	// 현재 순찰 대상, 배열로 갖게될것임
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	// 전투반경
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 3000.f;

	FTimerHandle PatrolTimer;  // 순찰 타이머
	
	// 최소순찰시간 최대순찰시간 배정
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	// 순찰 속도
	UPROPERTY(EditAnywhere, Category = "Combat")
	float patrollingSpeed = 300.f;
	
	FTimerHandle AttackTimer;
	
	FVector targetLoc;
	FVector startLoc;
	

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	// 순찰 타겟발견했을때의 속도
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 8.0f;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class EnemyAnimInstance* Anim;*/

	//UFUNCTION()
	//void AnimNotify_rushAttack1();
};