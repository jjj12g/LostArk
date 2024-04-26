// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h" // 이동
#include "EnhancedInputSubsystems.h" // 이동
#include "GameFramework/PlayerController.h" // 플레이어 컨트롤러
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "Components/CapsuleComponent.h"  // 플레이어 캡슐 컴포넌트
#include "GameFramework/CharacterMovementComponent.h" //캐릭터 이동
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



	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //플레이어 캡슐컴포넌트 크기설정

	//캐릭터가 카메라에 따라 회전하지않게조절
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar")); // 헬스바위젯
	HealthBarWidget->SetupAttachment(GetMesh()); //헬스바위젯을 루트로
	HealthBarWidget->SetRelativeLocation(FVector(0,0,240));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); //마우스랑 다를경우 초당 640만큼회전
	GetCharacterMovement()->bConstrainToPlane = true; // 캐릭터의 이동을 평면으로 고정
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // 캐릭터의 시작을 평면으로 시작되도록 고정

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


	// EnhacedInputSystem에서 입력 맵핑 콘텍스트 파일을 언리얼 엔진 시스템에 로드하기
	// 1. 현재 사용 중인 APlayerController 클래스 변수를 불러온다.
	 pc = GetWorld()->GetFirstPlayerController(); // 여기서 펄스트는 플레이어0번을 말함. pc는 플레이어컨트롤러의 약자
	/*
	int32 num; //이 경우라면 디폴트로 0이들어가 있을거임
	FString name = ""; // 이경우 ""라는 디폴트값이 들어가있음
	int32* number = nullptr; // 포인터는 주소를 불러와서 디폴트값이 널포인터로 들어가있음. 널포인터는 읽을 수 없음 그래서 어디를 가르킬지 몰라서 위험함.
	// 그래서 이경우 엔진이 꺼질 수도있음. Access Violence Error : Address 0x0 Read이런 문구가 나오게 됨 (nullptr가 참조되었다는 의미)*/

	// 2. EnhancedInput 내용을 담은 subsystem을 가져온다.
	if (pc != nullptr) // 보험, 널포인터가 아닐때 실행하도록 만듬 , 크래쉬가 안나기위한 방어코드.
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// 3. EnhancedInputLocalPlayerSubsystem에다가 imc 파일을 추가한다.
			subsys->AddMappingContext(imc_myMapping, 0);  //0 뒤의 옵션은 따로 안적어도 됨. 기본값이 따로 들어가있음.
		}
		pc->SetShowMouseCursor(true);  // 플레이중 마우스 커서 보이게
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is Null !"));
	}
	// ::은 스태틱함수를 의미함. 포인터를 따로 만들지않고 이름을 불러오는 것. 클래스파일로부터 직접함수를 불러올때 사용
	// ->은 포인터 변수로부터 불러올때


	targetPos = GetActorLocation();  // 일단 캐릭터의 위치에서부터 이동

	Tags.Add(FName("SlashCharacter")); // 캐릭터 태그이름	


	// 기본 체력 ui
	currentHP = MaxHP;
	PlayerWidget = Cast<UHealthBarWidget>(HealthBarWidget->GetWidget());

	currentMP = MaxMP;

	CurrentNum = MaxHealthNum;

	MPNum = MaxMPNum;

	PortionNumber;


	// main 위젯 생성 hp mp 실시간 설정

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

// 스킬 설정 및 위치 설정 -----------------------------------------------------------------------------------------------------

AActor* ASlashCharacter::ShootBullet()
{
	
	// 최대초와 현재초를 나눠서 퍼센트값에 넣어줘야함. -> float 변수?

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
	
	// 스킬 사용 시 마나 사용
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
	

	// 스킬 사용 시 마나 사용
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
	
	// 스킬 사용 시 마나 사용
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
	// 스킬 사용 시 마나 사용
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

	// 스킬 사용 체력 회복 테스스트
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

//--------------------------------------------------------------------------------------------------------------------------------- 스킬 설정 위치-----

// 데미지 시스템

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bAttackEnabled = false;
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, MaxHP);
	currentHP = FMath::Clamp(currentHP - DamageAmount, 0, MaxHP);
	CurrentNum = FMath::Clamp(CurrentNum - DamageAmount, 0, MaxHealthNum);

	// 넉백
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
		//	const int32 Selection = FMath::RandRange(0, 0); // 0~2까지가 3개
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
		//	const int32 Selection = FMath::RandRange(0, 0); // 0~2까지가 3개
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

		//페이드 인 효과를 준다.
		pc->PlayerCameraManager->StartCameraFade(0, 1, 1.5f, FLinearColor::Black);

		return;
		//ClientSetCameraFade(true, )  다른 방법 함수

		// 시작위치에서 다시 시작한다.
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



	//사용자가 입력한 방향대로 이동을 하고 싶다.
	//FVector moveDir = FVector(); //.도 ->와 비슷한것 ->는 변수가 포인터일때, .는 그냥 클래스가 인스턴스 일때 사용.

	FVector dir = targetPos - GetActorLocation(); // 타겟의 위치에서 나의 위치를 빼서
	
	if (dir.Length() > 100)   // 플레이어의 컬리전이 1미터쯤되니 1미터이상일때만 움직이도록
	{
		AddMovementInput(dir.GetSafeNormal());  // 마우스가 찍은 위치로
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
		// 이동 완료 후에는 RunMotion을 false로 설정
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = false;
	}


	//PlayerCharacter->SetActorRotation(ShootRot);

	if (bPlayerIsInvisible) // 플레이어가 쉬프트 투명 상태라면,
	{		
		// targetPos 목표 위치에 도달했을 때
		if (dir.Length() < 50)
		{
			// targetPos으로 플레이어의 위치를 설정하고
			SetActorLocation(targetPos);
			// Set Visibility를 true로 설정하여 플레이어의 모습을 다시 나타낸다.
			GetMesh()->SetVisibility(true, true);
			// 플레이어 이동속도 원상복귀
			GetCharacterMovement()->MaxWalkSpeed = 600;
			GetCharacterMovement()->MaxAcceleration = 2048;
			// 투명 상태 여부 초기화
			bPlayerIsInvisible = false;
		}
	}
	if (camrashake)
	{
		bcamerashake();
		camrashake = false;
	}

	// 트루일때 타이머 실행-> 몇초뒤 펄스로 변경 -> 틱에서 실행
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
	// 넉백
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
		// 함수를 인풋 컴포넌트에 연결한다.
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputDirection);  //앞에 접두사가 E로 시작하는건 이넘클래스(열거)로 이벤트처럼 선택지가 나오게 하는 클래스를 말함.
		//enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &ASlashCharacter::SetInputDirection); //컴플릿티드도 해줘야 누를때만 작동.
		//this 는 현재 값을 말함.
		// 다른클래스에서 똑같은 이름의 변수를 가져올 수 있으므로 파일명을 앞에 써주는게 좋음 AShootingPlayer::SetInputDirection처럼.
		enhancedInputComponent->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputJemp); // 일단 캐릭터 점프를 가져옴
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
	// direction의 방향으로 이동한다.
	// 이동 구성 요소: 방향, 속력, 시간 
	//FVector prevLocation = GetActorLocation(); // 현재위치값을 브리뷰 로케이션에 담는다. 원본에는 지장을주지않음.
	FVector nextLocation = GetActorLocation() + direction *1.0f * deltaTime;
	SetActorLocation(nextLocation, true);  // bSweep은 바닥을 쓸다라는 뜻으로 기본값 flase가 되어있음. true로 바꿔주면 앞에 뭐가있는지 체크를 하면서 이동함.

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime); 위에를 줄여쓰면 이렇게 사용가능.
}

void ASlashCharacter::ShiftStarted(const FInputActionValue& value)
{
	// 스페이스바 입력이 들어오면
	UE_LOG(LogTemp, Warning, (TEXT("spacebar")));
	niagaracomp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_space, GetActorLocation(), FRotator::ZeroRotator);

	// 플레이어 위치와 목표 좌표의 거리가 100이상일 때
	FVector distance = targetPos - GetActorLocation();
	if (distance.Length() > 80.0f && GetCharacterMovement()->GetLastUpdateVelocity().Length() > 0)
	{
		// Set Visibility를 false로 설정하여 플레이어의 모습을 감춘다.
		GetMesh()->SetVisibility(false, true);
		// 모습이 감춰져 있는 동안 이동 속도를 평상시보다 더 빠르게 설정한다.
		GetCharacterMovement()->MaxWalkSpeed = 1500;
		GetCharacterMovement()->MaxAcceleration = 4000;
		
		



		// 플레이어는 투명상태 : true로 설정
		bPlayerIsInvisible = true;
	}
}

void ASlashCharacter::Shiftsound(const FInputActionValue& value)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SPACESound);

}



// 좌클릭 바인딩 된 함수
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

	FHitResult hitInfo;   // 마우스가 히트되었을때

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
	{
		CachedDestination = hitInfo.ImpactPoint;    //히트된 좌표

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

			// 랜덤으로 몽타주 실행
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Fencing") + FString::FromInt(num);
			PlayAnimMontage(fencing_montage, 1.3, FName(sectionName));
		}

		if (w && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("W Mont")));

			bPlayerIsAttacking = true;

			// 랜덤으로 몽타주 실행
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Straight") + FString::FromInt(num);
			PlayAnimMontage(straight_montage, 1.3, FName(sectionName));
		}

		if (e && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("E Mont")));

			bPlayerIsAttacking = true;

			// 랜덤으로 몽타주 실행
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("HitGround") + FString::FromInt(num);
			PlayAnimMontage(hitground_montage, 1.3, FName(sectionName));
		}

		if (r && !bPlayerIsAttacking)
		{

			UE_LOG(LogTemp, Warning, (TEXT("R Mont")));

			bPlayerIsAttacking = true;

			// 랜덤으로 몽타주 실행
			int32 num = FMath::RandRange(3, 3);
			FString sectionName = FString("Sky") + FString::FromInt(num);
			PlayAnimMontage(sky_montage, 1.3, FName(sectionName));
		}
	}
}

// 스킬  QWERASDF 애니메이션 넣는 곳-----------------------------------------------------------
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
	// 사운드
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
	FString sectionName = FString("Fencing") + FString::FromInt(num); // FromInt : 숫자 변수의 값을 문자로 변환해주는 함수
	PlayAnimMontage(fencing_montage, 1, FName(sectionName));

	UE_LOG(LogTemp, Warning, TEXT("F___%d"), num);

	//playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	//playerAnim->bRunMotionOn = false;
}

// 여기 까지~~~ 스킬 ~~-------






void ASlashCharacter::SetInputDirection(const FInputActionValue& value)
{

	bool isPressed = value.Get<bool>();   // .이 캐스트의 의미 , 개발자가 어떻게 만들지 예측할 수 없어서 이런식으로 작성
	if (isPressed)
	{
		if (bPlayerIsAttacking)
		{
			return;
		}
		//FVector mousePos;
		//FVector mouseDir;
		//APlayerController->DeprojectMousePositionToWorld(mousePos, mouseDir);

		//마우스 커서 입력값 확인
		FVector WorldPosition, WorldDirection;
		APlayerController* MyController = Cast<APlayerController>(GetController());
		MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		FHitResult hitInfo;   // 마우스가 히트되었을때

		if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
		{
			targetPos = hitInfo.ImpactPoint;    //히트된 좌표
			targetPos.Z = GetActorLocation().Z;   //z 좌표를 플레이어의 좌표로 설정


			// 좌표 설정 확인
			DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);

		}

		//DrawDebugSphere(GetWorld(), mousePos, 1.0f, 5, FColor::Red, false, -1, 1, 1);

		// 이동 입력이 들어오면 RunMotion을 true로 설정
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
{	// 점프미완성
	bool isPressed = value.Get<bool>();
	if (isPressed)
	{
		//FVector SetActorLocation() = GetActorLocation() + FVector(-500, 100, 600);

	}
}
// 히트
void ASlashCharacter::hitreact()
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_JumpToSection(FName("hit"), HitbrathMotage);
	
}

// 데스
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






// 장비 시스템
//OverlappingWeapon->SetOwner(this);  // 무기가 장착되면 소유자로 인식
//OverlappingWeapon->SetInstigator(this);


/*   void overlapEvent   데미지 시스템
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
// 캐릭터 전투상태 만들기 E키를 눌렀을때로 설정해줘야함