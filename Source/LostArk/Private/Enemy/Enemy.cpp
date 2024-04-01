// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h" // AI ��Ʈ�ѷ�
#include "SlashCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� ������
#include "Perception/PawnSensingComponent.h" //�� �ð�û�� ������Ʈ
#include "NavigationData.h"  //NavPath����
#include "Navigation/PathFollowingComponent.h"
#include "NiagaraComponent.h" // ���̾ư��� ����Ʈ
#include "NiagaraFunctionLibrary.h" // ���̾ư���
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"  // ����� ȭ��ǥ �׸��� ��������
#include "Components/AttributeComponent.h"  // hp
#include "HUD/HealthBarComponent.h" //hp ��




#define	DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);  // ����� ��ü


AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // �ｺ������
	HealthBarWidget->SetupAttachment(GetRootComponent()); //�ｺ�������� ��Ʈ��



	// ���̾ư���



	


	// ĳ���� ������
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //�� ���۳�Ʈ �����ؼ� �����
	PawnSensing->SightRadius = 4000.f; // ���� ����Ʈ �ݰ�
	PawnSensing->SetPeripheralVisionAngle(45.f); // ���� �þ߰� �ݰ�
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

	if (PawnSensing) 
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	
}


bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	//ǥ�������� ��ġ
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

// �����ð� ������ �������� �̵�
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	{
		
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(50.f);
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
		 UE_LOG(LogTemp, Warning, TEXT("patroller"));
	}

	return nullptr;
}

void AEnemy::Attack()
{

	Super::Attack();
	PlayAttackMontage();
	
}

void AEnemy::PlayAttackMontage()
{

	Super::PlayAttackMontage();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0,2);  // ���� 3�����Ѱ� ����
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NI_Bossskill1,NiagaraComp,NAME_None,FVector(GetActorLocation()),FRotator(0.f),EAttachLocation::Type::KeepRelativeOffset,true);
			break;
			
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}


}

void AEnemy::PawnSeen(APawn* SeenPawn) // �÷��̾� �߰�
{
	if (EnemyState == EEnemyState::EES_Chasing) return; // Ÿ���̵��� �ѹ��� ȣ��ǵ���
	if (SeenPawn->ActorHasTag(FName("SlashCharacter"))) // �ν��� ��밡 ������ĳ���� �±װ� �ִ��� Ȯ��
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer); //Ÿ�̸ӵ� ���� �۵����ϰ� Ŭ�����Ŵ.
		GetCharacterMovement()->MaxWalkSpeed = 300.f; // �߰ݽ� �ӵ��� 300���� ��
		CombatTarget = SeenPawn;   // ����ǥ��

		if (EnemyState != EEnemyState::EES_Attacking) //���°� ���Ҷ����� ���ʹ� ������ �߰��ǹǷ� ���ʹ̰� ������ �����ϴ°� ���� ����, ��ǥ�� ������ �ؾ��ϹǷ� if�� �Ʒ��ʿ� ����
		{
			EnemyState = EEnemyState::EES_Chasing; //���� ������ ����
			MoveToTarget(CombatTarget); //  ���� �÷��̾���� �޷���
		}
		
		
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling) //������ �Ѿ�� ��Ȳ�� �Ǹ� ������� Ȯ��
	{
		CheckCombatTarget();  //����ǥ��Ȯ�� ��Ȳ�� �ʿ��� �� �ϱ�.
	}
	else
	{
		CheckPatrolTarget();  //���� ��ǥ������ ������ Ȯ�� (������� Ȯ��)
	}

}


void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))//��ǥ���� ����
	{
		PatrolTarget = ChoosePatrolTarget();  //�Ȱ����� �����ʰ��ϴ� �迭
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime); //�ּҿ� �ִ�ð� ���̷� ���ð��� ������
	}
}


void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius)) //�����ݰ� �ۿ������� �ȵǹǷ�!�� ����
	{
		// �÷��̾ �����ݰ� �ۿ������� ����ǥ���� ����(��ǥ��)
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling; // �ٽ� ����������� ����
		GetCharacterMovement()->MaxWalkSpeed = 125.f; // �� �� �ٽ� �ӵ��� 125����.
		MoveToTarget(PatrolTarget); // ����
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing) // �����ݰ� ���� ���ٸ�, �׸��� ���� �i���������������
	{
		 // ���� ������ ĳ����
		EnemyState = EEnemyState::EES_Chasing;  //  �߰ݸ�ǻ��
		GetCharacterMovement()->MaxWalkSpeed = 300.f; // ���꽺�ǵ� 300�����ϰ�
		MoveToTarget(CombatTarget); // ��ǥ���� �̵�
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking) // �÷��̾ ���ݹ����������� ���ݼ��� �׸���, ���ü����ݺ������ʱ����� ������
	{
		// �÷��̾ ���ݹ����ȿ� �ִٸ� ����
		EnemyState = EEnemyState::EES_Attacking;  // ���� ��� ���
		// TODO: ���ø��
		Attack();
		//UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
		
}





void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


			// ��Ʈ ���
void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	 PlayHitReactMontage(FName("FromLeft")); //�������� �޾����� ������ �ִϸ��̼�

	const FVector Forward = GetActorForwardVector(); // ���� ���� ����
	// �������� �޾��� ��, ���� �������� �ö��� �ʵ���
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal(); // Ÿ�� ����, Normal = ���� ��� ���͸� �����ͼ� ����ȭ���� �����Ѱ� ��Ʈ�� ��ȯ

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit); //Dot Product�� ���� 2���� ��Į�� ��ȯ
	// Take the inverse cosine (arc-cosine) of cos(theta) to get tgeta
	 double Theta = FMath::Acos(CosTheta);  // �� ������ ���� ��Ÿ
	// ������ ���� ������ �����Ƿ� ���ȿ��� ������ ��ȯ�ؾ���.
	Theta = FMath::RadiansToDegrees(Theta); // ������ ��Ÿ�� ������ ��ȯ

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	// ��Ÿ�� �����
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f); // ������ġ ������ġ�� ���溤�ʹ��ϰ� ���ϱ�60
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}


// ������ , ����ü�� ���
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		
	}
	return DamageAmount;
}





//float AEnemy::TakeDamege //(���� ������ ��������)
// ü�� �� �ٴ°� ǥ��
 //�÷��̾ ��ǥ�� ������ �ʾ������� �÷��̾� �ν�

 //CombatTarget = EventInstigator->GetPawn(); // ���� ��ġ�� ����
 //EnemyState == EEnemyState::EES_Chasing; // Ÿ������ �̵����·κ���
 //GetCharacterMovement()->MaxWalkSpeed = 300.f; // �̵��ӵ� 300���� ����
 //MoveToTarget (CombatTarget); //Ÿ�ٿ��� �̵�
 //return DamageAmount;

