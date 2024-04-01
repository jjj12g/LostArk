// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" //에너미와 플레이어의 공통부모
#include "interfaces/HitInterface.h"
#include "CharacterTypes.h" // 캐릭터 죽음같은 애니메이션 동작넣을때 사용할것.
#include "Enemy.generated.h"


class UPawnSensingComponent; //폰 감지센서 전방선언
class UAttributeComponent; // hp
class UHealthBarComponent; // hp 바

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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigetor, AActor* DamageCauser) override; // 데미지시스템



	
private:
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes; // hp

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp 바

	/*
	* Components
	*/


	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // 폰의 위치감지(시각, 청각) 컴포넌트 생성a

	








	// 대상
	UPROPERTY()
	AActor* CombatTarget;

	//이동속도
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f; 

	UPROPERTY(EditAnywhere)
	double AttackRadius = 300.f;   // 공격 반경

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

	// 전투반경
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;  // 순찰 타이머
	void PatrolTimerFinished(); // 순찰 타이머가 끝나면 실행

	
	

	// 최소순찰시간 최대순찰시간 배정
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	
	



protected:
	virtual void BeginPlay() override;

	// 타깃이 범위내에 있다면 true값 반환     수용반경
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);   //순찰시간끝나면 이동
	AActor* ChoosePatrolTarget(); //새 표적 선택하기
	virtual void Attack() override;  // 공격
	virtual void PlayAttackMontage() override; // 공격모션
	


	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // 델리게이트

	//UPROPERTY(BlueprintReadOnly)
	//EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling; //애니메이션 순찰에들어감

	



public:	
	
	


};
