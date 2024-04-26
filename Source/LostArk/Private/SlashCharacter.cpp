// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h" // �̵�
#include "EnhancedInputSubsystems.h" // �̵�
#include "GameFramework/PlayerController.h" // �÷��̾� ��Ʈ�ѷ�
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "Components/CapsuleComponent.h"  // �÷��̾� ĸ�� ������Ʈ
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� �̵�
#include "Tasks/AITask_MoveTo.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "followCameraActor.h"
#include "EngineUtils.h"
#include "Maactor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "MybulletActor.h"

//#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/Runtime/ModelViewViewModel/Source/ModelViewViewModelDebuggerEditor/Private/Widgets/SMainDebugTab.h"
#include "PlayerAnimInstance.h"
#include "HUD/HealthBarComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <HealthBarWidget.h>
#include "RealLostArkModeBase.h"
#include "Enemy/Enemy.h"
#include "MainWidget.h"
#include "Blueprint/UserWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimInstance.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/DamageEvents.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/Color.h>
#include <../../../../../../../Source/Runtime/Engine/Public/TimerManager.h>

ASlashCharacter::ASlashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;



	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //�÷��̾� ĸ��������Ʈ ũ�⼳��

	//ĳ���Ͱ� ī�޶� ���� ȸ�������ʰ�����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // �ｺ������
	HealthBarWidget->SetupAttachment(GetMesh()); //�ｺ�������� ��Ʈ��
	HealthBarWidget->SetRelativeLocation(FVector(0,0,240));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); //���콺�� �ٸ���� �ʴ� 640��ŭȸ��
	GetCharacterMovement()->bConstrainToPlane = true; // ĳ������ �̵��� ������� ����
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // ĳ������ ������ ������� ���۵ǵ��� ����

	staffMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Staff Mesh"));
	staffMeshComp->SetupAttachment(GetMesh(), FName("StaffSocket"));

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	springArmComp->TargetArmLength = 1000.0f;
	springArmComp->SetRelativeRotation(FRotator(-60.f, 350.f, 0.f));
	springArmComp->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);
	cameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm	
	cameraComp->SetRelativeLocation(FVector(30, 0, -550));
	cameraComp->SetRelativeRotation(FRotator(37, 0, 0));

	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("bullet spawn point"));
	SpawnLocation->SetupAttachment(GetMesh());

	mainWidget_bp = nullptr;

	mainWidget_inst = nullptr;


}



void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();


	// EnhacedInputSystem���� �Է� ���� ���ؽ�Ʈ ������ �𸮾� ���� �ý��ۿ� �ε��ϱ�
	// 1. ���� ��� ���� APlayerController Ŭ���� ������ �ҷ��´�.
	 pc = GetWorld()->GetFirstPlayerController(); // ���⼭ �޽�Ʈ�� �÷��̾�0���� ����. pc�� �÷��̾���Ʈ�ѷ��� ����
	/*
	int32 num; //�� ����� ����Ʈ�� 0�̵� ��������
	FString name = ""; // �̰�� ""��� ����Ʈ���� ������
	int32* number = nullptr; // �����ʹ� �ּҸ� �ҷ��ͼ� ����Ʈ���� �������ͷ� ������. �������ʹ� ���� �� ���� �׷��� ��� ����ų�� ���� ������.
	// �׷��� �̰�� ������ ���� ��������. Access Violence Error : Address 0x0 Read�̷� ������ ������ �� (nullptr�� �����Ǿ��ٴ� �ǹ�)*/

	// 2. EnhancedInput ������ ���� subsystem�� �����´�.
	if (pc != nullptr) // ����, �������Ͱ� �ƴҶ� �����ϵ��� ���� , ũ������ �ȳ������� ����ڵ�.
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// 3. EnhancedInputLocalPlayerSubsystem���ٰ� imc ������ �߰��Ѵ�.
			subsys->AddMappingContext(imc_myMapping, 0);  //0 ���� �ɼ��� ���� ����� ��. �⺻���� ���� ������.
		}
		pc->SetShowMouseCursor(true);  // �÷����� ���콺 Ŀ�� ���̰�
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is Null !"));
	}
	// ::�� ����ƽ�Լ��� �ǹ���. �����͸� ���� �������ʰ� �̸��� �ҷ����� ��. Ŭ�������Ϸκ��� �����Լ��� �ҷ��ö� ���
	// ->�� ������ �����κ��� �ҷ��ö�


	targetPos = GetActorLocation();  // �ϴ� ĳ������ ��ġ�������� �̵�

	Tags.Add(FName("SlashCharacter")); // ĳ���� �±��̸�	


	// �⺻ ü�� ui
	currentHP = MaxHP;
	PlayerWidget = Cast<UHealthBarWidget>(HealthBarWidget->GetWidget());

	currentMP = MaxMP;

	CurrentNum = MaxHealthNum;

	MPNum = MaxMPNum;

	PortionNumber;


	// main ���� ���� hp mp �ǽð� ����

		if (mainWidget_bp != nullptr)
	{
		mainWidget_inst = CreateWidget<UMainWidget>(GetWorld(), mainWidget_bp);

		/*if (mainWidget_inst)
		{
			mainWidget_inst->AddToViewport(0);
		}*/
	}

	mainWidget_inst->SetHealth(currentHP, MaxHP);
	mainWidget_inst->SetPower(currentMP, MaxMP);
	mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
	mainWidget_inst->SetPortionNUMS(currentHP,CurrentNum, PortionNumber);
	mainWidget_inst->SetCooltime(Currentcool, Maxcool);
}

// ��ų ���� �� ��ġ ���� -----------------------------------------------------------------------------------------------------

AActor* ASlashCharacter::ShootBullet()
{
	
	// �ִ��ʿ� �����ʸ� ������ �ۼ�Ʈ���� �־������. -> float ����?

	FVector toward = CachedDestination - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	
	if (BASESound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BASESound);
		UE_LOG(LogTemp, Warning, TEXT("BASESound !!"));
	}

	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet2()
{
	

	FVector toward = CachedDestination - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, CachedDestination, toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	
	// ��ų ��� �� ���� ���
	currentMP -= 30;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetPower(currentMP, MaxMP);
	}

	MPNum -= 30;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
	}
	
	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet3()
{
	if (!skilluse) { return nullptr; }
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn3, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	

	// ��ų ��� �� ���� ���
	currentMP -= 20;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetPower(currentMP, MaxMP);
	}

	MPNum -= 20;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
		mainWidget_inst->SetCooltime(skillCollTime, skillCollTimer);
	}

	skilluse = false;

	return SpawandActor;

}

AActor* ASlashCharacter::ShootBullet4()
{
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn4, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	
	// ��ų ��� �� ���� ���
	currentMP -= 25;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetPower(currentMP, MaxMP);
	}

	MPNum -= 25;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
	}
	
	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet5()
{
	FVector toward = CachedDestination - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn5, CachedDestination, toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	// ��ų ��� �� ���� ���
	currentMP -= 50;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetPower(currentMP, MaxMP);
	}

	MPNum -= 50;

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
	}


	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet6()
{
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn6, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);

	// ��ų ��� ü�� ȸ�� �׽���Ʈ
	PortionNumber -= 1;
	currentHP += 0.1;
	CurrentNum += 20;


	if (currentHP <= 1000 && CurrentNum <= 1000)
	{
		mainWidget_inst->SetPortionNUMS(currentHP, CurrentNum, PortionNumber);

	}

	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet7()
{
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn7, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);


	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet8()
{
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn8, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	return SpawandActor;
}

AActor* ASlashCharacter::ShootBullet9()
{
	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn9, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn2, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	return SpawandActor;
}

void ASlashCharacter::OnRepHealth()
{
	if (mainWidget_inst)
	{
		mainWidget_inst->SetHealth(currentHP, MaxHP);
	}

}

void ASlashCharacter::UpdateHealth(float DamageAmount)
{
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, MaxHP);

	if (currentHP == 0.f)
	{

	}

}

//--------------------------------------------------------------------------------------------------------------------------------- ��ų ���� ��ġ-----

// ������ �ý���

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bAttackEnabled = false;
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, MaxHP);
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, MaxHP);
	CurrentNum = FMath::Clamp(CurrentNum - DamageAmount, 0, MaxHealthNum);

	// �˹�
	/*if (binknockBack)
	{
	hitLocation = GetActorLocation();
	hitDirection = GetActorLocation() - enemy->GetActorLocation();
	hitDirection.Z = 0;
	hitDirection.Normalize();
	targetLoc = hitLocation + hitDirection * 50.0f;
	}*/

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetHealth(currentHP, MaxHP);
	}

	if (mainWidget_inst != nullptr)
	{
		mainWidget_inst->SetHealthNum(CurrentNum, MaxHealthNum);
	}


	
	if (PlayerWidget != nullptr)
	{

		PlayerWidget->SetHealthBar((float)currentHP / (float)MaxHP, FLinearColor(1.0f, 0.13f, 0.05f, 1.0f));
		
		int32 num = FMath::RandRange(1, 3);
		FString sectionName = FString("Damaged") + FString::FromInt(num);
		PlayAnimMontage(damaged_montage, 1.3, FName(sectionName));

		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//if (AnimInstance && HitbrathMotage)
		//{
		//	AnimInstance->Montage_Play(HitbrathMotage);
		//	const int32 Selection = FMath::RandRange(0, 0); // 0~2������ 3��
		//	FName SectionName = FName();
		//	switch (Selection)
		//	{
		//		
		//	case 0:
		//		hitreact();
		//		break;
		//	}
		//}
	}

	//currentHP -= DamageAmount;
	if (currentHP <= 0)
	{
		pc->PlayerCameraManager->StartCameraFade(0, 1, 1.5f, FLinearColor::Black);
		
		//CharacterState = ECharacterState::ECS_Die;

		PlayAnimMontage(died_montage);

		//CharacterState = ECharacterState::ECS_Die;

		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//if (AnimInstance && DeathMotages)
		//{
		//	AnimInstance->Montage_Play(DeathMotages);
		//	const int32 Selection = FMath::RandRange(0, 0); // 0~2������ 3��
		//	FName SectionName = FName();
		//	switch (Selection)
		//	{

		//	case 0:
		//		DeathMontage();
		//		break;
		//	}
		//}

	}


	//return DamageAmount;
	
	/*
	if (currentHP <= 0)
	{
			//AEnemy* enemy;
			//enemy -> removetarget();
		//attacker->enemyState = EEnemyState::RETURN;

		//���̵� �� ȿ���� �ش�.
		pc->PlayerCameraManager->StartCameraFade(0, 1, 1.5f, FLinearColor::Black);

		return;
		//ClientSetCameraFade(true, )  �ٸ� ��� �Լ�

		// ������ġ���� �ٽ� �����Ѵ�.
		//FTimerHandle restarthandle;
		//GetWorldTimerManager().SetTimer(restarthandle, FTimerDelegate::CreateLambda([&]() {
			//pc->UnPossess();
			//Cast<ARealLostArkModeBase>(GetWorld()->GetAuthGameMode())->RespawnPlayer(pc, this);
			//}), 1.5f, false);
	}*/

	//}
	return DamageAmount;
	
}





void ASlashCharacter::SetCanFire(bool value)
{
	CanFire = true;
}

void ASlashCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);



	//����ڰ� �Է��� ������ �̵��� �ϰ� �ʹ�.
	//FVector moveDir = FVector(); //.�� ->�� ����Ѱ� ->�� ������ �������϶�, .�� �׳� Ŭ������ �ν��Ͻ� �϶� ���.

	FVector dir = targetPos - GetActorLocation(); // Ÿ���� ��ġ���� ���� ��ġ�� ����
	
	if (dir.Length() > 100)   // �÷��̾��� �ø����� 1������Ǵ� 1�����̻��϶��� �����̵���
	{
		AddMovementInput(dir.GetSafeNormal());  // ���콺�� ���� ��ġ��
		if (bAttackEnabled)
		{
			//GetCharacterMovement()->DisableMovement();
			GetCharacterMovement()->DisableMovement();

			playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
			playerAnim->bRunMotionOn = false;
		}
		//GetCharacterMovement()->SetMovementMode();
		//GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	else
	{
		// �̵� �Ϸ� �Ŀ��� RunMotion�� false�� ����
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = false;
	}


	//PlayerCharacter->SetActorRotation(ShootRot);

	if (bPlayerIsInvisible) // �÷��̾ ����Ʈ ���� ���¶��,
	{		
		// targetPos ��ǥ ��ġ�� �������� ��
		if (dir.Length() < 50)
		{
			// targetPos���� �÷��̾��� ��ġ�� �����ϰ�
			SetActorLocation(targetPos);
			// Set Visibility�� true�� �����Ͽ� �÷��̾��� ����� �ٽ� ��Ÿ����.
			GetMesh()->SetVisibility(true, true);
			// �÷��̾� �̵��ӵ� ���󺹱�
			GetCharacterMovement()->MaxWalkSpeed = 600;
			GetCharacterMovement()->MaxAcceleration = 2048;
			// ���� ���� ���� �ʱ�ȭ
			bPlayerIsInvisible = false;
		}
	}
	if (camrashake)
	{
		bcamerashake();
		camrashake = false;
	}

	// Ʈ���϶� Ÿ�̸� ����-> ���ʵ� �޽��� ���� -> ƽ���� ����
	if (!skilluse)
	{

		if (skillCollTime < skillCollTimer)
		{
			skillCollTime += DeltaTime;
			skilluse = false;
		}
		else
		{
			skillCollTime = 0.0f;
			skilluse = true;
		}
	}

	if (manaTime < 5.0f)
	{
		manaTime += DeltaTime;
	}
	else
	{

		if (mainWidget_inst != nullptr)
		{
			mainWidget_inst->SetPower(currentMP, MaxMP);
			mainWidget_inst->SetMPNum(MPNum, MaxMPNum);
		}

		manaTime = 0;
		currentMP += 5;
		MPNum += 5;

	}
	// �˹�
	/*if (bknockBack)
	{
		bknobacktime += DeltaTime;
		if (bknobacktime < 3.0f)
		{
			binknockBack = false;
			FVector knockBackLocation = FMath::Lerp(GetActorLocation(), targetLoc, DeltaTime * 3.0f);
			if (FVector::Distance(GetActorLocation(), targetLoc) > 10)
			{
				SetActorLocation(knockBackLocation, true);
			}
		}
		else
		{
			binknockBack = true;
			bknobacktime = 0;
			bknockBack = false;
		}*/


}



void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);



	if (enhancedInputComponent != nullptr)
	{
		// �Լ��� ��ǲ ������Ʈ�� �����Ѵ�.
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputDirection);  //�տ� ���λ簡 E�� �����ϴ°� �̳�Ŭ����(����)�� �̺�Ʈó�� �������� ������ �ϴ� Ŭ������ ����.
		//enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &ASlashCharacter::SetInputDirection); //���ø�Ƽ�嵵 ����� �������� �۵�.
		//this �� ���� ���� ����.
		// �ٸ�Ŭ�������� �Ȱ��� �̸��� ������ ������ �� �����Ƿ� ���ϸ��� �տ� ���ִ°� ���� AShootingPlayer::SetInputDirectionó��.
		enhancedInputComponent->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputJemp); // �ϴ� ĳ���� ������ ������
		enhancedInputComponent->BindAction(ia_attack, ETriggerEvent::Started, this, &ASlashCharacter::Shoot);

		enhancedInputComponent->BindAction(ia_Fire, ETriggerEvent::Started, this, &ASlashCharacter::FireBullet);
		
		enhancedInputComponent->BindAction(ia_shift, ETriggerEvent::Started, this, &ASlashCharacter::Shiftsound);
		enhancedInputComponent->BindAction(ia_shift, ETriggerEvent::Triggered, this, &ASlashCharacter::ShiftStarted);

		enhancedInputComponent->BindAction(ia_q, ETriggerEvent::Started, this, &ASlashCharacter::Q);
		enhancedInputComponent->BindAction(ia_q, ETriggerEvent::Completed, this, &ASlashCharacter::Q);

		enhancedInputComponent->BindAction(ia_w, ETriggerEvent::Started, this, &ASlashCharacter::W);
		enhancedInputComponent->BindAction(ia_w, ETriggerEvent::Completed, this, &ASlashCharacter::W);

		enhancedInputComponent->BindAction(ia_e, ETriggerEvent::Started, this, &ASlashCharacter::FireBullet2);
		enhancedInputComponent->BindAction(ia_e, ETriggerEvent::Completed, this, &ASlashCharacter::FireBullet2);

		enhancedInputComponent->BindAction(ia_r, ETriggerEvent::Started, this, &ASlashCharacter::R);
		enhancedInputComponent->BindAction(ia_r, ETriggerEvent::Completed, this, &ASlashCharacter::R);

		enhancedInputComponent->BindAction(ia_a, ETriggerEvent::Started, this, &ASlashCharacter::PortionA);
		enhancedInputComponent->BindAction(ia_s, ETriggerEvent::Started, this, &ASlashCharacter::S);
		enhancedInputComponent->BindAction(ia_d, ETriggerEvent::Started, this, &ASlashCharacter::D);
		enhancedInputComponent->BindAction(ia_f, ETriggerEvent::Started, this, &ASlashCharacter::F);
	}



}

void ASlashCharacter::Move(FVector direction, float deltaTime)
{
	// direction�� �������� �̵��Ѵ�.
	// �̵� ���� ���: ����, �ӷ�, �ð� 
	//FVector prevLocation = GetActorLocation(); // ������ġ���� �긮�� �����̼ǿ� ��´�. �������� ��������������.
	FVector nextLocation = GetActorLocation() + direction *1.0f * deltaTime;
	SetActorLocation(nextLocation, true);  // bSweep�� �ٴ��� ���ٶ�� ������ �⺻�� flase�� �Ǿ�����. true�� �ٲ��ָ� �տ� �����ִ��� üũ�� �ϸ鼭 �̵���.

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime); ������ �ٿ����� �̷��� ��밡��.
}

void ASlashCharacter::ShiftStarted(const FInputActionValue& value)
{
	// �����̽��� �Է��� ������
	UE_LOG(LogTemp, Warning, (TEXT("spacebar")));
	niagaracomp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_space, GetActorLocation(), FRotator::ZeroRotator);

	// �÷��̾� ��ġ�� ��ǥ ��ǥ�� �Ÿ��� 100�̻��� ��
	FVector distance = targetPos - GetActorLocation();
	if (distance.Length() > 80.0f && GetCharacterMovement()->GetLastUpdateVelocity().Length() > 0)
	{
		// Set Visibility�� false�� �����Ͽ� �÷��̾��� ����� �����.
		GetMesh()->SetVisibility(false, true);
		// ����� ������ �ִ� ���� �̵� �ӵ��� ���ú��� �� ������ �����Ѵ�.
		GetCharacterMovement()->MaxWalkSpeed = 1500;
		GetCharacterMovement()->MaxAcceleration = 4000;
		
		



		// �÷��̾�� ������� : true�� ����
		bPlayerIsInvisible = true;
	}
}

void ASlashCharacter::Shiftsound(const FInputActionValue& value)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SPACESound);

}



// ��Ŭ�� ���ε� �� �Լ�
void ASlashCharacter::FireBullet(const FInputActionValue& value)
{
	bAttackEnabled = value.Get<bool>();

	if (bAttackEnabled)
	{
		UE_LOG(LogTemp, Warning, (TEXT("L Clicked")));
	}

	FVector WorldPosition, WorldDirection;
	APlayerController* MyController = Cast<APlayerController>(GetController());
	MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
	{
		CachedDestination = hitInfo.ImpactPoint;    //��Ʈ�� ��ǥ

		FVector dir = CachedDestination - GetActorLocation();

		FRotator newRot = UKismetMathLibrary::MakeRotFromZX(GetActorUpVector(), dir);
		SetActorRotation(newRot);

		if (!bKeyPressed && !bPlayerIsAttacking)
		{
			bPlayerIsAttacking = true;

			PlayAnimMontage(basic_montage);
		}

		if (q && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("Q Mont")));

			bPlayerIsAttacking = true;

			// �������� ��Ÿ�� ����
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Fencing") + FString::FromInt(num);
			PlayAnimMontage(fencing_montage, 1.3, FName(sectionName));
		}

		if (w && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("W Mont")));

			bPlayerIsAttacking = true;

			// �������� ��Ÿ�� ����
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Straight") + FString::FromInt(num);
			PlayAnimMontage(straight_montage, 1.3, FName(sectionName));
		}

		if (e && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("E Mont")));

			bPlayerIsAttacking = true;

			// �������� ��Ÿ�� ����
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("HitGround") + FString::FromInt(num);
			PlayAnimMontage(hitground_montage, 1.3, FName(sectionName));
		}

		if (r && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("R Mont")));

			bPlayerIsAttacking = true;

			// �������� ��Ÿ�� ����
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Sky") + FString::FromInt(num);
			PlayAnimMontage(sky_montage, 1.3, FName(sectionName));
		}
	}
}

// ��ų  QWERASDF �ִϸ��̼� �ִ� ��-----------------------------------------------------------
void ASlashCharacter::Q(const FInputActionValue& value)
{
	q = value.Get<bool>();

	if (q)
	{
		bKeyPressed = true;
		UE_LOG(LogTemp, Warning, (TEXT("Q preseed")));
	}
	else
	{
		bKeyPressed = false;
	}
	// ����
	if (QSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), QSound);
		UE_LOG(LogTemp, Warning, TEXT("Wsound!!"));
	}
}

void ASlashCharacter::W(const FInputActionValue& value)
{
	w = value.Get<bool>();

	if (w)
	{
		bKeyPressed = true;
		UE_LOG(LogTemp, Warning, (TEXT("W pressed")));
	}
	else
	{
		bKeyPressed = false;
	}
	if (WSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), WSound);
		UE_LOG(LogTemp, Warning, TEXT("Wsound!!"));
	}
}

void ASlashCharacter::FireBullet2(const FInputActionValue& value)
{
	e = value.Get<bool>();

	if (e)
	{
		bKeyPressed = true;
		UE_LOG(LogTemp, Warning, (TEXT("E preseed")));
	}
	else
	{
		bKeyPressed = false;
	}
	if (ESound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ESound);
		UE_LOG(LogTemp, Warning, TEXT("Esound!!"));
	}
}

void ASlashCharacter::R(const FInputActionValue& value)
{
	r = value.Get<bool>();

	if (r)
	{
		bKeyPressed = true;
		UE_LOG(LogTemp, Warning, (TEXT("R preseed")));
	}
	else
	{
		bKeyPressed = false;
	}
	if (RSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), RSound);
		UE_LOG(LogTemp, Warning, TEXT("Rsound!!"));
	}
}

void ASlashCharacter::PortionA(const FInputActionValue& value)
{
	ShootBullet6();
	UE_LOG(LogTemp, Warning, TEXT("A"));
}

void ASlashCharacter::S(const FInputActionValue& value)
{
	ShootBullet7();
	UE_LOG(LogTemp, Warning, TEXT("S"));
}

void ASlashCharacter::D(const FInputActionValue& value)
{
	ShootBullet8();
	UE_LOG(LogTemp, Warning, TEXT("D"));
}

void ASlashCharacter::F(const FInputActionValue& value)
{
	//ShootBullet8();
	bPlayerIsAttacking = true;

	int32 num = FMath::RandRange(1, 3);	
	FString sectionName = FString("Fencing") + FString::FromInt(num); // FromInt : ���� ������ ���� ���ڷ� ��ȯ���ִ� �Լ�
	PlayAnimMontage(fencing_montage, 1, FName(sectionName));

	UE_LOG(LogTemp, Warning, TEXT("F___%d"), num);

	//playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	//playerAnim->bRunMotionOn = false;
}

// ���� ����~~~ ��ų ~~-------






void ASlashCharacter::SetInputDirection(const FInputActionValue& value)
{

	bool isPressed = value.Get<bool>();   // .�� ĳ��Ʈ�� �ǹ� , �����ڰ� ��� ������ ������ �� ��� �̷������� �ۼ�
	if (isPressed)
	{
		if (bPlayerIsAttacking)
		{
			return;
		}
		//FVector mousePos;
		//FVector mouseDir;
		//APlayerController->DeprojectMousePositionToWorld(mousePos, mouseDir);

		//���콺 Ŀ�� �Է°� Ȯ��
		FVector WorldPosition, WorldDirection;
		APlayerController* MyController = Cast<APlayerController>(GetController());
		MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����

		if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
		{
			targetPos = hitInfo.ImpactPoint;    //��Ʈ�� ��ǥ
			targetPos.Z = GetActorLocation().Z;   //z ��ǥ�� �÷��̾��� ��ǥ�� ����


			// ��ǥ ���� Ȯ��
			DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);

		}

		//DrawDebugSphere(GetWorld(), mousePos, 1.0f, 5, FColor::Red, false, -1, 1, 1);

		// �̵� �Է��� ������ RunMotion�� true�� ����
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = true;

		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

}


void ASlashCharacter::bcamerashake()
{

	if (pc != nullptr)
	{
		//pc->ClientStopCameraShake(PlayerHitShake_bp);
		pc->ClientStartCameraShake(PlayerHitShake_bp);
	}

}

void ASlashCharacter::SetInputJemp(const FInputActionValue& value)
{	// �����̿ϼ�
	bool isPressed = value.Get<bool>();
	if (isPressed)
	{
		//FVector SetActorLocation() = GetActorLocation() + FVector(-500, 100, 600);

	}
}
// ��Ʈ
void ASlashCharacter::hitreact()
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("hit"), HitbrathMotage);
	
}

// ����
void ASlashCharacter::DeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("Death"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("Death"), DeathMotages);

}

void ASlashCharacter::Shoot(const FInputActionValue& value)
{
	

	Attack->PullTrigger();

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_BASIC, SetInputDirection(true), GetActorRotation());

}






// ��� �ý���
//OverlappingWeapon->SetOwner(this);  // ���Ⱑ �����Ǹ� �����ڷ� �ν�
//OverlappingWeapon->SetInstigator(this);


/*   void overlapEvent   ������ �ý���
*
*
*
*
*  UGamePlayStatics::ApplyDamage(
*		Box.Hit.GetActor(),
*		Damage,
*		GetInstigetor()->GetController(),
*		this,
*		UDamageType::StaticClass()
*		);
*
*/


// CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
// ĳ���� �������� ����� EŰ�� ���������� �����������