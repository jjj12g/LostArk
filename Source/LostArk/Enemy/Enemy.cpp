// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� ������
#include "AIController.h" // AI ��Ʈ�ѷ�
#include "NavigationData.h"  //NavPath����
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

	// ĳ���� ������
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

	//���ʹ� �����������

	EnemyController = Cast<AAIController>(GetController());  //AI ��Ʈ�ѷ��� ĳ��Ʈ����  

	MoveToTarget(PatrolTarget);

}


bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	//ǥ�������� ��ġ
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	GetActorLocation();
	Target->GetActorLocation();
	
	return DistanceToTarget <= Radius;
}

// �����ð� ������ �������� �̵�
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
	// ������� Ȯ��
	 
	//�Ȱ����� �����ʰ��ϴ� �迭
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

	//����ǥ��Ȯ�� ��Ȳ�� �ʿ��� �� �ϱ�.


		if (!InTargetRange(CombatTarget, CombatRadius)) //�����ݰ� �ۿ������� �ȵǹǷ�!�� ����
		{
			CombatTarget = nullptr;
			
		}
	

	//���� ��ǥ������ ������ Ȯ�� (������� Ȯ��)
	if (InTargetRange(PatrolTarget, PatrolRadius))//��ǥ���� ����
	{
		PatrolTarget = ChoosePatrolTarget();  //�Ȱ����� �����ʰ��ϴ� �迭
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f); //5�ʵڴ���Ÿ������ ȣ��
	}
}






void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

