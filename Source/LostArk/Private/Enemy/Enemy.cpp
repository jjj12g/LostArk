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
#include "textnibox.h" // �׽�Ʈ
#include "slowhitActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h>



#define	DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8.f, 12, Color, false, 5.f);  // ����� ��ü


AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	// �Ž��ݸ��� �ϴ���� ���ݸ������� �α� �ڽ��ø���Ȯ���غ��� �ٲܻ���
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("EnemyPreset"));
	GetMesh()->SetGenerateOverlapEvents(true);
	// ���ʹ� ���������� ����
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

	// ĳ���� ������
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing")); //�� ���۳�Ʈ �����ؼ� �����
	PawnSensing->SightRadius = 4000.f; // ���� ����Ʈ �ݰ�
	PawnSensing->SetPeripheralVisionAngle(45.f); // ���� �þ߰� �ݰ�

	// ���̾ư��� ���� ��ġ
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

	// �⺻���� ���͸��� ���� ������ x �Ķ���� ���·θ� ���� ����
	// �޽��� ���͸����� DynamicMaterial�� ���� ���ش�
	UMaterialInterface* currentMat = GetMesh()->GetMaterial(0);
	dynamicMAT = UMaterialInstanceDynamic::Create(currentMat, nullptr);
	GetMesh()->SetMaterial(0, dynamicMAT);

	// �ι�° ���͸��� �� ����
	UMaterialInterface* currentMat2 = GetMesh()->GetMaterial(1);
	dynamicMAT1 = UMaterialInstanceDynamic::Create(currentMat2, nullptr);
	GetMesh()->SetMaterial(1, dynamicMAT1);

	/* �⺻ ���� , ���������� x
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		//AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		//DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		//EquippedWeapon = DefaultWeapon;
	}
	*/

	





	// ü�º��� �ʱ�ȭ
	// ü�� ������ �ʱ�ȭ�Ѵ�.
	currentHP = maxHP;
	//���� ������Ʈ�� �Ҵ�Ǿ� �ִ� ���� �ν��Ͻ��� �����´�.
	EnemyWidget = Cast<UHealthBarWidget>(floatingWidgetComp->GetWidget());

	// ���忡 �ִ� �÷��̾ ã�´�
	for (TActorIterator<ASlashCharacter>player(GetWorld()); player; ++player)
	{
		target = *player; // Ŭ������ ������
	}

	MoveToTarget(target);
	EnemyState = EEnemyState::EES_Patrolling;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	floatingWidgetComp->SetWorldRotation(BillboradwidgetComponent(target));

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

	//UE_LOG(LogTemp, Warning, TEXT("State Transition: %s"), *UEnum::GetValueAsString<EEnemyState>(EnemyState)); // ���罺�����ͽ� Ȯ��

	// ��������
	if (rush1 == true)
	{
		rushAttack(DeltaTime);
	}

	if (breathDamage)
	{
		ShootBullet();
	 breathDamage = false;
	}
	// �극��
	if (breath1 == true)
	{
		if (NI_breath != nullptr)
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_breath, SpawnLocation->GetComponentLocation(), SpawnLocation->GetComponentRotation(), FVector(2.0f));
			// �� �Ҹ� ��ġ Ÿ�ָ̹°� �Ű��ֱ�
			
		breath1 = false;
	}

	// ���ݽ� ������ ����
	if (dontMove == true)
	{
		EnemyController->StopMovement();
	}
	// ������ �� ������ �����̱� ����
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

	// ������ ȸ��
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

	// ������ slowrains�� �Լ��� ����� ����
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

		// �ؿ� ��Ʈ ����
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




// ������ , ����ü�� ���
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
		CombatTarget = EventInstigetor->GetPawn(); // ���� ���ظ� �Դ� ��� ������ǥ ����
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
	




	// ��Ʈ ���
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
//	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
//	ShowHealthBar(); // ü�¹� ���̰� ����
//
//	// ����ִٸ� ��Ʈ��Ÿ��
//	if (currentHP > 0.0f)
//	{
//		DirectionalHitReact(ImpactPoint);
//	}
//	 //�װԾƴϸ� �������
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
	// �������
	PlayDeathMontages();
	ClearAttackTimer(); // �������̸� ���ݸ���
	// ������ ü�¹� ���� �Ⱥ��̰� ����
	//HideHealthBar();
	DisableCapsule(); // ���ʹ̰� �׾����� �ݸ����� ������� ��.
	SetLifeSpan(DeathLifeSpan); // ���ʹ̰� ������ n�� �ڿ� �����
	GetCharacterMovement()->bOrientRotationToMovement = false; // ���� �� �ٸ��������� �������� �ʰ� ����


}

void AEnemy::Attack()
{
	// ������ �÷��̾�Է� ����ȸ��
	FVector lookDir = target->GetActorLocation() - GetActorLocation();
	moveDir = lookDir.GetSafeNormal();  //��ֶ���� ����� ���� ���̰� 1�̵�.
	FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), moveDir);  // ù��° ���� ������ �ι�° ���� ���߷��� ��
	rotStart = GetActorRotation();
	rotTarget = newRot;
	bLookTarget = true;

	// ���� �� ���͸��� �� ����
	dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(1, 1, 1, 100));
	// �ι�° �� ����
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
			const int32 Selection = FMath::RandRange(1, 1); // 0~2������ 3��
			FName SectionName = FName();
			switch (Selection)
			{
				// ����
			case 0:
				AttackMontage1();
				break;

				// �극��
			case 1:
				AttackMontage2();
				break;

				// ���� �ѹ���
			case 2:
				AttackMontage3();
				break;

				// �ٴ���� ����ġ��
			case 3:
				AttackMontage4();
				break;

				// �ٴ� 3�����
			case 4:
				AttackMontage5();
				break;

				// �翷 ��������
			case 5:
				AttackMontage6();
				break;

				// ������ 
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
			const int32 Selection = FMath::RandRange(1, 1); // 0~2������ 3��
			FName SectionName = FName();
			switch (Selection)
			{
				// ����
			case 0:
				AttackMontage1();
				break;

				// �극��
			case 1:
				AttackMontage2();
				break;

				// ���� �ѹ���
			case 2:
				AttackMontage3();
				break;

				// �ٴ���� ����ġ��
			case 3:
				AttackMontage4();
				break;

				// �ٴ� 3�����
			case 4:
				AttackMontage5();
				break;

				// �翷 ��������
			case 5:
				AttackMontage6();
				break;

				// ������
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
	// ���� �� ���͸��� �� ����
	//dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(100, 95, 0,100));
	// �ι�° �� ����
	//dynamicMAT1->SetVectorParameterValue(FName("hit Color"), FVector4(100, 95, 0, 100));
	// Ÿ�� ����
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
	// ��������
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
	// ���� �� ���͸��� �� ����
	dynamicMAT->SetVectorParameterValue(FName("Hit color"), FVector4(0, 0, 0,100));
	// �ι�° �� ����
	dynamicMAT1->SetVectorParameterValue(FName("hit Color"), FVector4(0, 0, 0, 100));

}

void AEnemy::AttackMontage6()
{
	UE_LOG(LogTemp, Warning, TEXT("attack6"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Attack6"), AttackMontage);

}

// ������
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
	//  ��ü ��ġ
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
		const int32 Selection = FMath::RandRange(0, 0); // 0~2������ 3��
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
	EnemyController = Cast<AAIController>(GetController());  //AI ��Ʈ�ѷ��� ĳ��Ʈ����  
	MoveToTarget(PatrolTarget); //���ʹ� �����������
	//HideHealthBar();
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
/*
void AEnemy::HideHealthBar()
{
	if (floatingWidgetComp)
	{
		floatingWidgetComp->SetVisibility(false); // �ǰ�ǥ�ñ� ������ ����� �ٽü���
	}
}*/
// ������������ �ｺ�� ����
void AEnemy::ShowHealthBar()
{
	if (floatingWidgetComp)
	{

		floatingWidgetComp->SetVisibility(true); // �ǰ�ǥ�ñ� ������ ����� �ٽü���
	}
}
// �׾����� �ｺ�� ����
void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	//HideHealthBar();
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
	return DistanceToTarget <= 1500;//Radius;
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

