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
#include "PlayerAnimInstance.h"


ASlashCharacter::ASlashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;



	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //플레이어 캡슐컴포넌트 크기설정

	//캐릭터가 카메라에 따라 회전하지않게조절
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("bullet spawn point"));
	SpawnLocation->SetupAttachment(GetMesh());



}



void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();


	// EnhacedInputSystem에서 입력 맵핑 콘텍스트 파일을 언리얼 엔진 시스템에 로드하기
	// 1. 현재 사용 중인 APlayerController 클래스 변수를 불러온다.
	APlayerController* pc = GetWorld()->GetFirstPlayerController(); // 여기서 펄스트는 플레이어0번을 말함. pc는 플레이어컨트롤러의 약자
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

	/*
	// 월드상의 뷰 타겟을 자동으로 찾아서 실행시켜줌
	for (TActorIterator<AfollowCameraActor> it(GetWorld()); it; ++it)
	{
		AfollowCameraActor* mainCam = *it;
		GetController<APlayerController>()->SetViewTarget(mainCam); // 뷰타겟을 찾음
		break;
	}
	*/
	/*
	// 지팡이 붙이기
	Attack=GetWorld()->SpawnActor<AMaactor>(Attackclass);
	GetMesh()->HideBoneByName(TEXT("Attack"),EPhysBodyOp::PBO_None);
	Attack->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform, TEXT("Attack"));
	Attack->SetOwner(this);
	*/
}

AActor* ASlashCharacter::ShootBullet()
{

	FVector toward = targetPos - GetActorLocation();
	FVector loc = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	//AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), toward.Rotation(), SpawnParams);
	AActor* SpawandActor = GetWorld()->SpawnActor<AMybulletActor>(bullettospawn, SpawnLocation->GetComponentLocation(), GetActorRotation(), SpawnParams);
	return SpawandActor;

}

// 데미지 시스템
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HP -= DamageAmount;
	if (HP <= 0)
	{
		Destroy();
	}
	return DamageAmount;
}
//입력바인딩 통해 발사
void ASlashCharacter::FireBullet(const FInputActionValue& value)
{
	//FVector dirction = FVector(value.Get<FVector2D>(),0);
	//ShootRot = dirction.Rotation();
	ShootBullet();
	//PlayerCharacter->SetActorRotation(dirction.Rotation());
	/*

	if(PlayerCharacter && CanFire)
	{
		PlayerCharacter->ShootBullet();
		CanFire = false;

		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ASlashCharacter::SetCanFire,true);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimeBetweenFire,false);
	}
	*/

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
	}
	// 회전 값 주기

	else {
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = false;
	}

	//PlayerCharacter->SetActorRotation(ShootRot);
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
	}



}
void ASlashCharacter::Move(FVector direction, float deltaTime)
{
	// direction의 방향으로 이동한다.
	// 이동 구성 요소: 방향, 속력, 시간 
	FVector prevLocation = GetActorLocation(); // 현재위치값을 브리뷰 로케이션에 담는다. 원본에는 지장을주지않음.
	FVector nextLocation = prevLocation + direction * speed * deltaTime;
	SetActorLocation(nextLocation, true);  // bSweep은 바닥을 쓸다라는 뜻으로 기본값 flase가 되어있음. true로 바꿔주면 앞에 뭐가있는지 체크를 하면서 이동함.

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime); 위에를 줄여쓰면 이렇게 사용가능.

}
void ASlashCharacter::SetInputDirection(const FInputActionValue& value)
{

	bool isPressed = value.Get<bool>();   // .이 캐스트의 의미 , 개발자가 어떻게 만들지 예측할 수 없어서 이런식으로 작성
	if (isPressed)
	{
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

		// 월드 포지션z를 캐릭터캡슐컬리전으로 두고
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = true;











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