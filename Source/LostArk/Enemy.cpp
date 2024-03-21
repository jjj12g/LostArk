// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� ������
#include "AIController.h" // AI ��Ʈ�ѷ�
#include "NavigationData.h"  //NavPath����
#include "Navigation/PathFollowingComponent.h"



AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// ĳ���� ������
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//���ʹ� �����������
	EnemyController = Cast<AAIController>(GetController());  //AI ��Ʈ�ѷ��� ĳ��Ʈ����
	if (EnemyController && PatrolTarget)   //���ʹ� ��Ʈ�ѷ��� ��ȿ���� Ȯ��
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
	//ǥ�������� ��ġ
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���� ���� Ž��

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

