// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h" // AI 컨트롤러
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 움직임
#include "Perception/PawnSensingComponent.h" //폰 시각청각 컴포넌트
#include "Components/AttributeComponent.h" //hp
#include "HUD/HealthBarComponent.h" //hp 바
#include "NavigationData.h"  //NavPath참조
#include "Navigation/PathFollowingComponent.h"
#include "items/Weapons/Weapon.h"
#include "DrawDebugHelpers.h"
#include "EnemyAnimInstance.h"
#include "TimerManager.h"
#include "Components/Boxcomponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>




#define	DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);  // 디버그 구체


AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	// 매쉬콜리전 일단잠시 노콜리전으로 두기 박스컬리전확인해보고 바꿀생각
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("EnemyPreset"));
	//GetMesh()->SetGenerateOverlapEvents(true);
	// 에너미 프리셋으로 변경
	//GetMesh()->SetCollisionProfileName(FName("Enemypreset"));
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	//boxComp->SetGenerateOverlapEvents(true);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // 헬스바위젯
	HealthBarWidget->SetupAttachment(GetRootComponent()); //헬스바위젯을 루트로



	// 캐릭터 움직임
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //폰 컴퍼넌트 생성해서 만들기
	PawnSensing->SightRadius = 4000.f; // 폰의 사이트 반경
	PawnSensing->SetPeripheralVisionAngle(45.f); // 폰의 시야각 반경
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//  전투타겟이 널포인터인지 확인
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling) //순찰을 넘어서는 상황이 되면 전투대상 확인
	{
		CheckCombatTarget();  //전투표적확인 상황에 필요한 일 하기.
	}
	else
	{
		CheckPatrolTarget();  //순찰 목표범위에 들어갔는지 확인 (순찰대상 확인)
	}

	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *UEnum::GetValueAsString<EEnemyState>(EnemyState)); // 현재스테이터스 확인


	// 러쉬어택
	if (rush1 == true)
	{ 
		rushAttack(DeltaTime);
	}
	
	// 브레스
	if (breath1 == true)
	{
		if (NI_breath != nullptr)
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_breath, GetActorLocation()-FVector(0,-100,-100), FRotator(0, 90, 0));
		breath1 = false;
	}
	if (EnemyoverlapOn == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("enemy collsion on!"));
		enemyCollisionOn();
		if (EnemyoveralpOff == true)
		{ 
			//UE_LOG(LogTemp, Warning, TEXT("enemy collsion off!"));
			enemyCollisionOff();
			EnemyoverlapOn = false;
			EnemyoveralpOff = false;
		}
	}



}

// 데미지 , 남은체력 계산
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	
	HP -= DamageAmount;
	if (HP <= 0.0f)
	{
		Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("enemy hit!"));
	//return DamageAmount;
	HandleDamage(DamageAmount);
	if (player == nullptr)
	{
	CombatTarget = EventInstigetor->GetPawn(); // 적이 피해를 입는 즉시 전투목표 설정
	ChaseTarget();
	}
	return DamageAmount;
}

// Destroyed 무기나 필요없을듯.

	// 히트 모션
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	ShowHealthBar(); // 체력바 보이게 설정

	// 살아있다면 히트몽타주
	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	// 그게아니면 데스모션
	else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	
	//GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BeginOverlap);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BeginOverlap);


	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();



	/* 기본 무기 , 지워도오류 x
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
	// 죽음모션
	PlayDeathMontage();
	ClearAttackTimer(); // 공격중이면 공격멈춤
	// 죽으면 체력바 위젯 안보이게 설정
	HideHealthBar();
	DisableCapsule(); // 에너미가 죽엇을때 콜리전을 사라지게 함.
	SetLifeSpan(DeathLifeSpan); // 에너미가 죽으면 n초 뒤에 사라짐
	GetCharacterMovement()->bOrientRotationToMovement = false; // 죽은 뒤 다른방향으로 움직이지 않게 고정


}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 0); // 0~2까지가 3개
		FName SectionName = FName();
		switch (Selection)
		{
			// 돌진 공격
		case 0:	
			AttackMontage1();
			break;
			// 브레스
		case 1:
			AttackMontage2();
			break;
			// 위로 한바퀴
		case 2:
			AttackMontage3();
			break;
			// 뒤로꼬리치고 몸들었다가 다시꼬리치기
		case 3:
			AttackMontage4();
			break;
			// 바닥찍고 꼬리치기
		case 4:
			AttackMontage5();
			break;
			// 바닥 3번찍기
		case 5:
			AttackMontage6();
			break;
			// 양옆 감전날개
		case 6:
			AttackMontage7();
			break;
		case 7:
			AttackMontage8();
			break;
		case 8:
			AttackMontage9();
			break;
		default:
			AttackMontage10();
			break;
		}


	}
	//PlayAttackMontage();
	//PlayRandomMontageSection(AttackMontage,AttackMontageSections);
	//if (PlayRandomMontageSection)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("1 !"));
	//}
}

void AEnemy::AttackMontage1()
{
	UE_LOG(LogTemp, Warning, TEXT("attack1"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack1"), AttackMontage);
	
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Lighting, GetActorLocation()-FVector(0,0,400), FRotator(0, 90, 0));
			
	// 타겟 범위
	startLoc = GetActorLocation();
	targetLoc = GetActorLocation()+ GetActorForwardVector() * 500;
	
	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *UEnum::GetValueAsString<EEnemyState>(EnemyState));
}

bool AEnemy::rushAttack(float deltaSeconds)
{

	// 러쉬어택
	stackTime += deltaSeconds;

	FVector rushLocation = FMath::Lerp(startLoc, targetLoc, stackTime);

	SetActorLocation(rushLocation);
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Bossskill1, GetActorLocation() - FVector(0, 0, 400), FRotator(0,90,0));
	//if(FVector::Distance(GetActorLocation(), targetLoc) < 10.0f){
	//	resh = false;
	//}
	if(stackTime >= 1.0f){
		stackTime = 0.0f;
		
		rush1=false;
	}

	if (rush2 == true)
	{
		// 콜리전 없애기.
		
	}

	//EEnemyState::EES_Attacking;
	return true;
}

void AEnemy::AttackMontage2()
{
	UE_LOG(LogTemp, Warning, TEXT("attack2"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack2"), AttackMontage);

	


}

void AEnemy::AttackMontage3()
{
	UE_LOG(LogTemp, Warning, TEXT("attack3"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack3"), AttackMontage);

}

void AEnemy::AttackMontage4()
{
	UE_LOG(LogTemp, Warning, TEXT("attack4"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack4"), AttackMontage);

}

void AEnemy::AttackMontage5()
{
	UE_LOG(LogTemp, Warning, TEXT("attack5"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack5"), AttackMontage);

}

void AEnemy::AttackMontage6()
{
	UE_LOG(LogTemp, Warning, TEXT("attack6"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack6"), AttackMontage);

}

void AEnemy::AttackMontage7()
{
	UE_LOG(LogTemp, Warning, TEXT("attack7"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack7"), AttackMontage);

}

void AEnemy::AttackMontage8()
{
	UE_LOG(LogTemp, Warning, TEXT("attack8"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack8"), AttackMontage);

}

void AEnemy::AttackMontage9()
{
	UE_LOG(LogTemp, Warning, TEXT("attack9"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack9"), AttackMontage);

}
void AEnemy::AttackMontage10()
{
	UE_LOG(LogTemp, Warning, TEXT("attack10"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack10"), AttackMontage);

}

void AEnemy::enemyCollisionOn()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);

}

void AEnemy::enemyCollisionOff()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Ignore);

}

void AEnemy::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamegeEnemy"));
	//AController* aiCT = GetInstigator()->GetController();
	//AController* Playerc = GetInstigator()->GetController();
	//AAIController* enemyCon = EnemyController;

	UGameplayStatics::ApplyDamage(OtherActor, 30, EnemyController,  this, DamageType);
	enemy = Cast<AEnemy>(OtherActor);

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

int32 AEnemy::PlayDeathMontage() // int32로 부모클래스와 결과를 동일하게 설정 
 {
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);  // TEnum 열거체
	if (Pose < EDeathPose::EDP_MAX) // 배열의 요소의 수 확인 (Enum 값보다 높은지)
	{
		DeathPose = Pose;
	}

	return Selection;
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());  //AI 컨트롤러로 캐스트해줌  
	MoveToTarget(PatrolTarget); //에너미 순찰경로지정
	HideHealthBar();
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))//목표지점 도달
	{
		PatrolTarget = ChoosePatrolTarget();  //똑같은거 뽑지않게하는 배열
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime); //최소와 최대시간 사이로 대기시간이 정해짐
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius()) //전투반경 밖에있으면 안되므로!를 붙임
	{
		ClearAttackTimer();
		// 플레이어가 전투반경 밖에있으면 전투표적을 없앰(널표시)
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();


	}
	else if (IsOutsideAttackRadius() && !IsChasing()) // 전투반경 내에 없다면, 그리고 아직 쫒고있지않은경우라면
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}

	else if (CanAttack()) // 플레이어가 공격범위에있을때 공격설정 그리고, 어택설정반복하지않기위해 엔드사용
	{
		StartAttackTimer();
		// TODO: 어택모션
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
		HealthBarWidget->SetVisibility(false); // 건강표시기 위젯을 숨기고 다시순찰
	}
}
// 가까이있을때 헬스바 보임
void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{

		HealthBarWidget->SetVisibility(true); // 건강표시기 위젯을 숨기고 다시순찰
	}
}
// 죽었을때 헬스바 숨김
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}
// 순찰시작
void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling; // 다시 순찰모션으로 변경
	GetCharacterMovement()->MaxWalkSpeed = patrollingSpeed; // 그 후 다시 속도를 125로함.
	MoveToTarget(PatrolTarget); // 순찰

}
// 추격
void AEnemy::ChaseTarget()
{
	
	EnemyState = EEnemyState::EES_Chasing;  //  추격모션사용
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed; // 무브스피드 300으로하고
	MoveToTarget(CombatTarget); // 목표물로 이동
	
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
// 에너미 상태가 추격인지 확인
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

// 에너미 상태가 공격상태인지 확인
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
	GetWorldTimerManager().ClearTimer(PatrolTimer); //타이머도 이제 작동안하게 클리어시킴.

}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer); // 플레이어가 순찰지점 밖으로나가면 어택타이머 작동안하게 클리어
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	//표적까지의 위치
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

// 순찰시간 끝나면 다음으로 이동
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
		UE_LOG(LogTemp, Warning, TEXT("patroller"));
	}

	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn) // 플레이어 추격
{
	const bool bShouldChaseTarget =   // 전부맞을때만 true
		EnemyState != EEnemyState::EES_Dead && // 타겟이 죽은상태가 아니여야함.
		EnemyState != EEnemyState::EES_Chasing && // 타겟이동은 한번만 호출되도록
		EnemyState < EEnemyState::EES_Attacking && //상태가 변할때마다 에너미 공격이 추가되므로 에너미가 여러번 공격하는거 방지 또한, 목표를 정한후 해야하므로 if문 아래쪽에 설정
		SeenPawn->ActorHasTag(FName("SlashCharacter")); // 인식한 상대가 슬래시캐릭터 태그가 있는지 확인

	if (bShouldChaseTarget) // 목표를 추적중인지 확인
	{
		CombatTarget = SeenPawn;   // 전투표적
		ClearPatrolTimer();
		ChaseTarget(); //  적이 플레이어를보면 달려옴
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

// 어택 나이아가라
//SectionName = FName("Attack2");
//NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Bossskill1, GetActorLocation(), FRotator::ZeroRotator);














