// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 움직임
#include "AIController.h" // AI 컨트롤러
#include "NavigationData.h"  //NavPath참조
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"



AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// 캐릭터 움직임
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}


void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//에너미 순찰경로지정

	EnemyController = Cast<AAIController>(GetController());  //AI 컨트롤러로 캐스트해줌  

	MoveToTarget(PatrolTarget);

}


bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	//표적까지의 위치
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	GetActorLocation();
	Target->GetActorLocation();
	
	return DistanceToTarget <= Radius;
}

// 순찰시간 끝나면 다음으로 이동
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(15.f);
		EnemyController->MoveTo(MoveRequest);
		
	}
	
}

AActor* AEnemy::ChoosePatrolTarget()
{
	// 순찰대상 확인
	 
	//똑같은거 뽑지않게하는 배열
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}


	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		 return ValidTargets[TargetSelection];
	}

	return nullptr;
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//전투표적확인 상황에 필요한 일 하기.


		if (!InTargetRange(CombatTarget, CombatRadius)) //전투반경 밖에있으면 안되므로!를 붙임
		{
			CombatTarget = nullptr;
			
		}
	

	//순찰 목표범위에 들어갔는지 확인 (순찰대상 확인)
	if (InTargetRange(PatrolTarget, PatrolRadius))//목표지점 도달
	{
		PatrolTarget = ChoosePatrolTarget();  //똑같은거 뽑지않게하는 배열
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f); //5초뒤다음타깃으로 호출
	}
}






void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

