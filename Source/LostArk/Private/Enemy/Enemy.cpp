// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h" // AI 컨트롤러
#include "SlashCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 움직임
#include "Perception/PawnSensingComponent.h" //폰 시각청각 컴포넌트
#include "NavigationData.h"  //NavPath참조
#include "Navigation/PathFollowingComponent.h"
#include "NiagaraComponent.h" // 나이아가라 이펙트
#include "NiagaraFunctionLibrary.h" // 나이아가라
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"  // 디버그 화살표 그리기 가져오기
#include "Components/AttributeComponent.h"  // hp
#include "HUD/HealthBarComponent.h" //hp 바




#define	DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);  // 디버그 구체


AEnemy::AEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // 헬스바위젯
	HealthBarWidget->SetupAttachment(GetRootComponent()); //헬스바위젯을 루트로



	// 나이아가라



	


	// 캐릭터 움직임
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //폰 컴퍼넌트 생성해서 만들기
	PawnSensing->SightRadius = 4000.f; // 폰의 사이트 반경
	PawnSensing->SetPeripheralVisionAngle(45.f); // 폰의 시야각 반경
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

	if (PawnSensing) 
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	
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
		const int32 Selection = FMath::RandRange(0,2);  // 공격 3개중한개 랜덤
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

void AEnemy::PawnSeen(APawn* SeenPawn) // 플레이어 추격
{
	if (EnemyState == EEnemyState::EES_Chasing) return; // 타겟이동은 한번만 호출되도록
	if (SeenPawn->ActorHasTag(FName("SlashCharacter"))) // 인식한 상대가 슬래시캐릭터 태그가 있는지 확인
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer); //타이머도 이제 작동안하게 클리어시킴.
		GetCharacterMovement()->MaxWalkSpeed = 300.f; // 추격시 속도를 300으로 함
		CombatTarget = SeenPawn;   // 전투표적

		if (EnemyState != EEnemyState::EES_Attacking) //상태가 변할때마다 에너미 공격이 추가되므로 에너미가 여러번 공격하는거 방지 또한, 목표를 정한후 해야하므로 if문 아래쪽에 설정
		{
			EnemyState = EEnemyState::EES_Chasing; //만약 맞으면 추적
			MoveToTarget(CombatTarget); //  적이 플레이어를보면 달려옴
		}
		
		
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling) //순찰을 넘어서는 상황이 되면 전투대상 확인
	{
		CheckCombatTarget();  //전투표적확인 상황에 필요한 일 하기.
	}
	else
	{
		CheckPatrolTarget();  //순찰 목표범위에 들어갔는지 확인 (순찰대상 확인)
	}

}


void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))//목표지점 도달
	{
		PatrolTarget = ChoosePatrolTarget();  //똑같은거 뽑지않게하는 배열
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime); //최소와 최대시간 사이로 대기시간이 정해짐
	}
}


void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius)) //전투반경 밖에있으면 안되므로!를 붙임
	{
		// 플레이어가 전투반경 밖에있으면 전투표적을 없앰(널표시)
		CombatTarget = nullptr;
		EnemyState = EEnemyState::EES_Patrolling; // 다시 순찰모션으로 변경
		GetCharacterMovement()->MaxWalkSpeed = 125.f; // 그 후 다시 속도를 125로함.
		MoveToTarget(PatrolTarget); // 순찰
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing) // 전투반경 내에 없다면, 그리고 아직 쫒고있지않은경우라면
	{
		 // 공격 범위밖 캐릭터
		EnemyState = EEnemyState::EES_Chasing;  //  추격모션사용
		GetCharacterMovement()->MaxWalkSpeed = 300.f; // 무브스피드 300으로하고
		MoveToTarget(CombatTarget); // 목표물로 이동
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking) // 플레이어가 공격범위에있을때 공격설정 그리고, 어택설정반복하지않기위해 엔드사용
	{
		// 플레이어가 공격범위안에 있다면 공격
		EnemyState = EEnemyState::EES_Attacking;  // 공격 모션 사용
		// TODO: 어택모션
		Attack();
		//UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
		
}





void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


			// 히트 모션
void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	 PlayHitReactMontage(FName("FromLeft")); //데미지를 받았을시 나오는 애니메이션

	const FVector Forward = GetActorForwardVector(); // 적의 전방 진로
	// 데미지를 받았을 때, 몸이 공중으로 올라가지 않도록
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal(); // 타격 지점, Normal = 뺄셈 결과 벡터를 가져와서 정규화한후 저장한걸 히트에 반환

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit); //Dot Product는 백터 2개와 스칼라를 반환
	// Take the inverse cosine (arc-cosine) of cos(theta) to get tgeta
	 double Theta = FMath::Acos(CosTheta);  // 둘 사이의 각도 세타
	// 단위를 라디안 각도로 얻으므로 라디안에서 각도를 변환해야함.
	Theta = FMath::RadiansToDegrees(Theta); // 라디안은 세타와 라디안을 전환

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	// 세타값 디버그
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f); // 최종위치 적의위치에 전방벡터더하고 곱하기60
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}


// 데미지 , 남은체력 계산
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		
	}
	return DamageAmount;
}





//float AEnemy::TakeDamege //(적이 공격을 당했을시)
// 체력 바 다는거 표시
 //플레이어가 목표에 들어오지 않았을때도 플레이어 인식

 //CombatTarget = EventInstigator->GetPawn(); // 적의 위치를 추적
 //EnemyState == EEnemyState::EES_Chasing; // 타겟으로 이동상태로변경
 //GetCharacterMovement()->MaxWalkSpeed = 300.f; // 이동속도 300으로 설정
 //MoveToTarget (CombatTarget); //타겟에게 이동
 //return DamageAmount;

