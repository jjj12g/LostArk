// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h" // AI ��Ʈ�ѷ�
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� ������
#include "Perception/PawnSensingComponent.h" //�� �ð�û�� ������Ʈ
#include "Components/AttributeComponent.h" //hp
#include "HUD/HealthBarComponent.h" //hp ��
#include "NavigationData.h"  //NavPath����
#include "Navigation/PathFollowingComponent.h"
#include "items/Weapons/Weapon.h"
#include "DrawDebugHelpers.h"


#define	DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);  // ����� ��ü


AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);




	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // �ｺ������
	HealthBarWidget->SetupAttachment(GetRootComponent()); //�ｺ�������� ��Ʈ��



	// ĳ���� ������
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //�� ���۳�Ʈ �����ؼ� �����
	PawnSensing->SightRadius = 4000.f; // ���� ����Ʈ �ݰ�
	PawnSensing->SetPeripheralVisionAngle(45.f); // ���� �þ߰� �ݰ�
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//  ����Ÿ���� ������������ Ȯ��
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) //������ �Ѿ�� ��Ȳ�� �Ǹ� ������� Ȯ��
	{
		CheckCombatTarget();  //����ǥ��Ȯ�� ��Ȳ�� �ʿ��� �� �ϱ�.
	}
	else
	{
		CheckPatrolTarget();  //���� ��ǥ������ ������ Ȯ�� (������� Ȯ��)
	}

}

// ������ , ����ü�� ���
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	HP -= DamageAmount;
	if (HP <= 0)
	{
		Destroy();
	}
	//return DamageAmount;
	//HandleDamage(DamageAmount);
	CombatTarget = EventInstigetor->GetPawn(); // ���� ���ظ� �Դ� ��� ������ǥ ����
	ChaseTarget();
	return DamageAmount;
}

// Destroyed ���⳪ �ʿ������.

	// ��Ʈ ���
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	ShowHealthBar(); // ü�¹� ���̰� ����

	// ����ִٸ� ��Ʈ��Ÿ��
	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	// �װԾƴϸ� �������
	else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();



	/* �⺻ ���� , ���������� x
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		//DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		//EquippedWeapon = DefaultWeapon;
	}
	*/

}

void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	// �������
	PlayDeathMontage();
	ClearAttackTimer(); // �������̸� ���ݸ���
	// ������ ü�¹� ���� �Ⱥ��̰� ����
	HideHealthBar();
	DisableCapsule(); // ���ʹ̰� �׾����� �ݸ����� ������� ��.
	SetLifeSpan(DeathLifeSpan); // ���ʹ̰� ������ n�� �ڿ� �����
	GetCharacterMovement()->bOrientRotationToMovement = false; // ���� �� �ٸ��������� �������� �ʰ� ����


}

void AEnemy::Attack()
{

	EnemyState = EEnemyState::EES_Engaged;
	Super::Attack();
	//PlayAttackMontage();
	PlayRandomMontageSection(AttackMontage, AttackMontageSections);

}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();

}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());

	}

}

int32 AEnemy::PlayDeathMontage() // int32�� �θ�Ŭ������ ����� �����ϰ� ���� 
 {
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);  // TEnum ����ü
	if (Pose < EDeathPose::EDP_MAX) // �迭�� ����� �� Ȯ�� (Enum ������ ������)
	{
		DeathPose = Pose;
	}

	return Selection;
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());  //AI ��Ʈ�ѷ��� ĳ��Ʈ����  
	MoveToTarget(PatrolTarget); //���ʹ� �����������
	HideHealthBar();
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))//��ǥ���� ����
	{
		PatrolTarget = ChoosePatrolTarget();  //�Ȱ����� �����ʰ��ϴ� �迭
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime); //�ּҿ� �ִ�ð� ���̷� ���ð��� ������
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius()) //�����ݰ� �ۿ������� �ȵǹǷ�!�� ����
	{
		ClearAttackTimer();
		// �÷��̾ �����ݰ� �ۿ������� ����ǥ���� ����(��ǥ��)
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();


	}
	else if (IsOutsideAttackRadius() && !IsChasing()) // �����ݰ� ���� ���ٸ�, �׸��� ���� �i���������������
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}

	else if (CanAttack()) // �÷��̾ ���ݹ����������� ���ݼ��� �׸���, ���ü����ݺ������ʱ����� ������
	{
		StartAttackTimer();
		// TODO: ���ø��
		//Attack();

		//UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}

}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);

}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false); // �ǰ�ǥ�ñ� ������ ����� �ٽü���
	}
}
// ������������ �ｺ�� ����
void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{

		HealthBarWidget->SetVisibility(true); // �ǰ�ǥ�ñ� ������ ����� �ٽü���
	}
}
// �׾����� �ｺ�� ����
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}
// ��������
void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling; // �ٽ� ����������� ����
	GetCharacterMovement()->MaxWalkSpeed = patrollingSpeed; // �� �� �ٽ� �ӵ��� 125����.
	MoveToTarget(PatrolTarget); // ����

}
// �߰�
void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;  //  �߰ݸ�ǻ��
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed; // ���꽺�ǵ� 300�����ϰ�
	MoveToTarget(CombatTarget); // ��ǥ���� �̵�
}

bool AEnemy::IsOutsideCombatRadius()
{
	return  !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);

}

bool AEnemy::IsInsideAttackRadius()
{

	return InTargetRange(CombatTarget, AttackRadius);
}
// ���ʹ� ���°� �߰����� Ȯ��
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

// ���ʹ� ���°� ���ݻ������� Ȯ��
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;

}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;

}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer); //Ÿ�̸ӵ� ���� �۵����ϰ� Ŭ�����Ŵ.

}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer); // �÷��̾ �������� �����γ����� ����Ÿ�̸� �۵����ϰ� Ŭ����
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

void AEnemy::PawnSeen(APawn* SeenPawn) // �÷��̾� �߰�
{
	const bool bShouldChaseTarget =   // ���θ������� true
		EnemyState != EEnemyState::EES_Dead && // Ÿ���� �������°� �ƴϿ�����.
		EnemyState != EEnemyState::EES_Chasing && // Ÿ���̵��� �ѹ��� ȣ��ǵ���
		EnemyState < EEnemyState::EES_Attacking && //���°� ���Ҷ����� ���ʹ� ������ �߰��ǹǷ� ���ʹ̰� ������ �����ϴ°� ���� ����, ��ǥ�� ������ �ؾ��ϹǷ� if�� �Ʒ��ʿ� ����
		SeenPawn->ActorHasTag(FName("SlashCharacter")); // �ν��� ��밡 ������ĳ���� �±װ� �ִ��� Ȯ��

	if (bShouldChaseTarget) // ��ǥ�� ���������� Ȯ��
	{
		CombatTarget = SeenPawn;   // ����ǥ��
		ClearPatrolTimer();
		ChaseTarget(); //  ���� �÷��̾���� �޷���
	}

}






/*void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();
	if (Animinstance && HitReactMontage)
	{
		Animinstance->Montage_Play(HitReactMontage);
		Animinstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}

}*/

// ���� ���̾ư���
//SectionName = FName("Attack2");
//NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Bossskill1, GetActorLocation(), FRotator::ZeroRotator);














