// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 움직임
#include "AIController.h" // AI 컨트롤러
#include "NavigationData.h"  //NavPath참조
#include "Navigation/PathFollowingComponent.h"



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


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//에너미 순찰경로지정
	EnemyController = Cast<AAIController>(GetController());  //AI 컨트롤러로 캐스트해줌
	if (EnemyController && PatrolTarget)   //에너미 컨트롤러가 유효한지 확인
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}


	}
	
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	//표적까지의 위치
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//순찰 지점 탐색

	if (CombatTarget)
	{
		if (InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
		}
	}
	if (PatrolTarget)



}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

