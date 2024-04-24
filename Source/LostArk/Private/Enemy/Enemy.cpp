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
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstance.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "Components/CapsuleComponent.h"
#include "SlashCharacter.h"
#include "HealthBarWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "EngineUtils.h"
#include "BaseFloatingText.h"
#include "Components/TextRenderComponent.h"
#include "MybulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/World.h>
#include "textnibox.h" // 테스트
#include "slowhitActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h>



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
	GetMesh()->SetGenerateOverlapEvents(true);
	// 에너미 프리셋으로 변경
	//GetMesh()->SetCollisionProfileName(FName("Enemypreset"));
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	//boxComp->SetGenerateOverlapEvents(true);
	//GetMesh()->SetGenerateOverlapEvents(true);

	floatingWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingWidgetComp"));
	floatingWidgetComp->SetupAttachment(GetMesh());
	floatingWidgetComp->SetRelativeLocation(FVector(240, 0, 400));
	//floatingWidgetComp->SetRelativeRotation(FRotator(0, 0, 90));
	floatingWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	floatingWidgetComp->SetDrawSize(FVector2D(150, 100));

	// 캐릭터 움직임
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //폰 컴퍼넌트 생성해서 만들기
	PawnSensing->SightRadius = 4000.f; // 폰의 사이트 반경
	PawnSensing->SetPeripheralVisionAngle(45.f); // 폰의 시야각 반경

	// 나이아가라 스폰 위치
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("bullet spawn point"));
	SpawnLocation->SetupAttachment(GetMesh());


	Dash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("niagara comp"));
	Dash->SetupAttachment(GetCapsuleComponent());

	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//for (TActorIterator<ASlashCharacter> players(GetWorld()); players; ++players)
	//{
	//	target = *players;
	//}

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::BeginOverlap);

	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();

	// 기본적인 머터리얼 색상 변경이 x 파라미터 형태로만 접근 가능
	// 메시의 머터리얼을 DynamicMaterial로 변경 해준다
	UMaterialInterface* currentMat = GetMesh()->GetMaterial(0);
	dynamicMAT = UMaterialInstanceDynamic::Create(currentMat, nullptr);
	GetMesh()->SetMaterial(0, dynamicMAT);

	// 두번째 머터리얼 색 변경
	UMaterialInterface* currentMat2 = GetMesh()->GetMaterial(1);
	dynamicMAT1 = UMaterialInstanceDynamic::Create(currentMat2, nullptr);
	GetMesh()->SetMaterial(1, dynamicMAT1);

	/* 기본 무기 , 지워도오류 x
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		//DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		//EquippedWeapon = DefaultWeapon;
	}
	*/

	





	// 체력변수 초기화
	// 체력 변수를 초기화한다.
	currentHP = maxHP;
	//위젯 컴포넌트에 할당되어 있는 위젯 인스턴스를 가져온다.
	EnemyWidget = Cast<UHealthBarWidget>(floatingWidgetComp->GetWidget());

	// 월드에 있는 플레이어를 찾는다
	for (TActorIterator<ASlashCharacter>player(GetWorld()); player; ++player)
	{
		target = *player; // 클래스의 다형성
	}

	MoveToTarget(target);
	EnemyState = EEnemyState::EES_Patrolling;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	floatingWidgetComp->SetWorldRotation(BillboradwidgetComponent(target));

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

	if (breathDamage)
	{
		ShootBullet();
	 breathDamage = false;
	}
	// 브레스
	if (breath1 == true)
	{
		if (NI_breath != nullptr)
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_breath, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));
			// 슛 불릿 위치 타이밍맞게 옮겨주기
			
		breath1 = false;
	}

	// 공격시 움직임 멈춤
	if (dontMove == true)
	{
		EnemyController->StopMovement();
	}
	// 공격이 다 끝나면 움직이기 가능
	if (look == true)
	{
		stackTime += DeltaTime;
		if (stackTime >= 2.3f) 
		{
			stackTime = 0.0f;
			UE_LOG(LogTemp, Warning, TEXT("look!"));
			MoveToTarget(target);
			look = false;
		}
	}

	// 공격전 회전
	if (bLookTarget)
	{
		if (rotTime < 1.0f)
		{
			rotTime += DeltaTime;
			SetActorRotation(FMath::Lerp(rotStart, rotTarget, rotTime));
			//UE_LOG(LogTemp, Warning, TEXT("111111111111111"));
		}
		else
		{
			rotTime = 0;
			targetLoc = GetActorLocation() + GetActorForwardVector() * 1500;
			threeAttack = true;
			bLookTarget = false;
		}
	}

	// 느린비 slowrains로 함수로 만들까 생각
	if (btestTarget)
	{
		teste();
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e1, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e2, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e3, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e4, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e5, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation()+e6, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation() + e7, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation() + e8, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation() + e9, GetActorRotation());
		GetWorld()->SpawnActor<Atextnibox>(testspawn, GetActorLocation() + e10, GetActorRotation());

		// 밑에 히트 스폰
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e1 + FVector(0,0,-266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e2 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e3 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e4 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e5 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e6 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e7 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e8 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e9 + FVector(0, 0, -266), GetActorRotation());
		GetWorld()->SpawnActor<AslowhitActor>(testhitbullet, GetActorLocation() + e10 + FVector(0, 0, -266), GetActorRotation());
		//UE_LOG(LogTemp, Warning, TEXT("123123"));
		btestTarget = false;
	}
	
		if (threeAttack)
		{
			if (threeAttacks)
			{ 
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_SHOWKWAVE, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation());

			threeAttack = false;
			threeAttacks = false;
			}
		}


}	




// 데미지 , 남은체력 계산
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigetor, AActor* DamageCauser)
{
	if (bDie) {return 0.0f;}
		
		
	/*
	HP -= DamageAmount;
	if (HP <= 0.0f)
	{
		Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("enemy hit!"));
	//return DamageAmount;
	HandleDamage(DamageAmount);
	if (target == nullptr)
	{
		CombatTarget = EventInstigetor->GetPawn(); // 적이 피해를 입는 즉시 전투목표 설정
		ChaseTarget();
	}
	return DamageAmount;*/
	
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, maxHP);
	if (EnemyWidget != nullptr)
	{
		EnemyWidget->SetHealthBar((float)currentHP / (float)maxHP, FLinearColor(1.0f, 0.13f, 0.05f, 1.0f));
		UE_LOG(LogTemp, Warning, TEXT("enemy hit!"));
	}

	if (currentHP <= 0.0f)
	{
		Die();
	}
	return 0.0f;
}
	




	// 히트 모션
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
//	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
//	ShowHealthBar(); // 체력바 보이게 설정
//
//	// 살아있다면 히트몽타주
//	if (currentHP > 0.0f)
//	{
//		DirectionalHitReact(ImpactPoint);
//	}
//	 //그게아니면 데스모션
//	else 
//	{
//	Die();
//	PlayHitSound(ImpactPoint);
//	SpawnHitParticles(ImpactPoint);
//	}
}


void AEnemy::Die()
{
	bDie = true;
	EnemyState = EEnemyState::EES_Dead;
	UE_LOG(LogTemp,Warning,TEXT("die"));
	// 죽음모션
	PlayDeathMontages();
	ClearAttackTimer(); // 공격중이면 공격멈춤
	// 죽으면 체력바 위젯 안보이게 설정
	//HideHealthBar();
	DisableCapsule(); // 에너미가 죽엇을때 콜리전을 사라지게 함.
	SetLifeSpan(DeathLifeSpan); // 에너미가 죽으면 n초 뒤에 사라짐
	GetCharacterMovement()->bOrientRotationToMovement = false; // 죽은 뒤 다른방향으로 움직이지 않게 고정


}

void AEnemy::Attack()
{
	// 공격전 플레이어에게로 방향회전
	FVector lookDir = target->GetActorLocation() - GetActorLocation();
	moveDir = lookDir.GetSafeNormal();  //노멀라이즈를 해줘야 백터 길이가 1이됨.
	FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), moveDir);  // 첫번째 축은 고정축 두번째 축은 맞추려는 축
	rotStart = GetActorRotation();
	rotTarget = newRot;
	bLookTarget = true;

	// 공격 시 머터리얼 색 변경
	dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(1, 1, 1, 100));
	// 두번째 색 변경
	dynamicMAT1->SetVectorParameterValue(FName("hit Color"), FVector4(1, 1, 1, 100));

	EnemyState = EEnemyState::EES_Engaged;
	look = false;
	dontMove = true;

	
	if (FVector::Distance(target->GetActorLocation(), GetActorLocation()) > attackDistance)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			AnimInstance->Montage_Play(AttackMontage);
			const int32 Selection = FMath::RandRange(1, 1); // 0~2까지가 3개
			FName SectionName = FName();
			switch (Selection)
			{
				// 돌진
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

				// 바닥찍고 꼬리치기
			case 3:
				AttackMontage4();
				break;

				// 바닥 3번찍기
			case 4:
				AttackMontage5();
				break;

				// 양옆 감전날개
			case 5:
				AttackMontage6();
				break;

				// 느린비 
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
			threeAttack = false;
		}
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && AttackMontage)
		{
			AnimInstance->Montage_Play(AttackMontage);
			const int32 Selection = FMath::RandRange(1, 1); // 0~2까지가 3개
			FName SectionName = FName();
			switch (Selection)
			{
				// 돌진
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

				// 바닥찍고 꼬리치기
			case 3:
				AttackMontage4();
				break;

				// 바닥 3번찍기
			case 4:
				AttackMontage5();
				break;

				// 양옆 감전날개
			case 5:
				AttackMontage6();
				break;

				// 느린비
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
			threeAttack = false;
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
	if (Dash != nullptr)
	{
		Dash->Activate();
	}

	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Lighting, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation());
	// 공격 시 머터리얼 색 변경
	//dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(100, 95, 0,100));
	// 두번째 색 변경
	//dynamicMAT1->SetVectorParameterValue(FName("hit Color"), FVector4(100, 95, 0, 100));
	// 타겟 범위
	startLoc = GetActorLocation();
	//targetLoc = GetActorLocation() + GetActorForwardVector() * 1500;
	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *UEnum::GetValueAsString<EEnemyState>(EnemyState));
}



AActor* AEnemy::ShootBullet()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	return SpawandActor;
}

bool AEnemy::rushAttack(float deltaSeconds)
{
	// 러쉬어택
	stackTime += deltaSeconds;

	FVector rushLocation = FMath::Lerp(startLoc, targetLoc, stackTime);
	SetActorRotation(rotTarget);
	SetActorLocation(rushLocation);
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Bossskill1, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation());

	//if(FVector::Distance(GetActorLocation(), targetLoc) < 10.0f){
	//	resh = false;
	//}

	if (stackTime >= 1.0f) {
		stackTime = 0.0f;
		rush1 = false;
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
	
	threeAttacks = true;
	// 공격 시 머터리얼 색 변경
	dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(0, 0, 0,100));
	// 두번째 색 변경
	dynamicMAT1->SetVectorParameterValue(FName("hit Color"), FVector4(0, 0, 0, 100));

}

void AEnemy::AttackMontage6()
{
	UE_LOG(LogTemp, Warning, TEXT("attack6"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack6"), AttackMontage);

}

// 느린비
void AEnemy::AttackMontage7()
{
	UE_LOG(LogTemp, Warning, TEXT("attack7"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack7"), AttackMontage);

	
	btestTarget = true;
	bteste = true;
	
}

void AEnemy::teste()
{
	//  구체 위치
	e1 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e2 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e3 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, -300), 0);
	e4 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e5 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e6 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e7 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e8 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e9 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);
	e10 = FVector(FMath::RandRange(-1500, 1500), FMath::RandRange(-1500, 1500), 0);

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

void AEnemy::DeathMontage1()
{
	UE_LOG(LogTemp, Warning, TEXT("Death1"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Death1"), DeathMontage);
}



void AEnemy::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (EnemyoverlapOn == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamegeEnemy"));
		
		UGameplayStatics::ApplyDamage(OtherActor, 30, EnemyController, this, DamageType);
		enemy = Cast<AEnemy>(OtherActor);
		EnemyoverlapOn = false;

	}

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

void AEnemy::PlayDeathMontages()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(0, 0); // 0~2까지가 3개
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			DeathMontage1();
			break;
		default:
			break;
		}
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();

}
/*
void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes && floatingWidgetComp)
	{
		floatingWidgetComp->SetHealthPercent(Attributes->GetHealthPercent());
	}

}*/

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());  //AI 컨트롤러로 캐스트해줌  
	MoveToTarget(PatrolTarget); //에너미 순찰경로지정
	//HideHealthBar();
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
/*
void AEnemy::HideHealthBar()
{
	if (floatingWidgetComp)
	{
		floatingWidgetComp->SetVisibility(false); // 건강표시기 위젯을 숨기고 다시순찰
	}
}*/
// 가까이있을때 헬스바 보임
void AEnemy::ShowHealthBar()
{
	if (floatingWidgetComp)
	{

		floatingWidgetComp->SetVisibility(true); // 건강표시기 위젯을 숨기고 다시순찰
	}
}
// 죽었을때 헬스바 숨김
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	//HideHealthBar();
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
	return DistanceToTarget <= 1500;//Radius;
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

FRotator AEnemy::BillboradwidgetComponent(AActor* camActor)
{
	//UE_LOG(LogTemp,Warning,TEXT("1111111111111111111"));
	players = Cast<ASlashCharacter>(target);
	if (players != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("222222222222221111"));
		FVector lookDir = (players->cameraComp->GetComponentLocation() - floatingWidgetComp->GetComponentLocation()).GetSafeNormal();
		//FRotator lookRot = UKismetMathLibrary::MakeRotFromX(lookDir);
		FRotator lookRot = lookDir.ToOrientationRotator();

		return lookRot;
	}
	else
	{	
		//UE_LOG(LogTemp, Warning, TEXT("33333333333111"));
		return FRotator::ZeroRotator;
	}

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

