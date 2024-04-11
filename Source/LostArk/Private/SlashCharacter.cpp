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
#include "PlayerAnimInstance.h"


ASlashCharacter::ASlashCharacter()
{

	PrimaryActorTick.bCanEverTick = true;



	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //�÷��̾� ĸ��������Ʈ ũ�⼳��

	//ĳ���Ͱ� ī�޶� ���� ȸ�������ʰ�����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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


	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("bullet spawn point"));
	SpawnLocation->SetupAttachment(GetMesh());



}



void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();


	// EnhacedInputSystem���� �Է� ���� ���ؽ�Ʈ ������ �𸮾� ���� �ý��ۿ� �ε��ϱ�
	// 1. ���� ��� ���� APlayerController Ŭ���� ������ �ҷ��´�.
	APlayerController* pc = GetWorld()->GetFirstPlayerController(); // ���⼭ �޽�Ʈ�� �÷��̾�0���� ����. pc�� �÷��̾���Ʈ�ѷ��� ����
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

	/*
	// ������� �� Ÿ���� �ڵ����� ã�Ƽ� ���������
	for (TActorIterator<AfollowCameraActor> it(GetWorld()); it; ++it)
	{
		AfollowCameraActor* mainCam = *it;
		GetController<APlayerController>()->SetViewTarget(mainCam); // ��Ÿ���� ã��
		break;
	}
	*/
	/*
	// ������ ���̱�
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

// ������ �ý���
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HP -= DamageAmount;
	if (HP <= 0)
	{
		Destroy();
	}
	return DamageAmount;
}
//�Է¹��ε� ���� �߻�
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



	//����ڰ� �Է��� ������ �̵��� �ϰ� �ʹ�.
	//FVector moveDir = FVector(); //.�� ->�� ����Ѱ� ->�� ������ �������϶�, .�� �׳� Ŭ������ �ν��Ͻ� �϶� ���.

	FVector dir = targetPos - GetActorLocation(); // Ÿ���� ��ġ���� ���� ��ġ�� ����

	if (dir.Length() > 100)   // �÷��̾��� �ø����� 1������Ǵ� 1�����̻��϶��� �����̵���
	{
		AddMovementInput(dir.GetSafeNormal());  // ���콺�� ���� ��ġ��
	}
	// ȸ�� �� �ֱ�

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
		// �Լ��� ��ǲ ������Ʈ�� �����Ѵ�.
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputDirection);  //�տ� ���λ簡 E�� �����ϴ°� �̳�Ŭ����(����)�� �̺�Ʈó�� �������� ������ �ϴ� Ŭ������ ����.
		//enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Completed, this, &ASlashCharacter::SetInputDirection); //���ø�Ƽ�嵵 ����� �������� �۵�.
		//this �� ���� ���� ����.
		// �ٸ�Ŭ�������� �Ȱ��� �̸��� ������ ������ �� �����Ƿ� ���ϸ��� �տ� ���ִ°� ���� AShootingPlayer::SetInputDirectionó��.
		enhancedInputComponent->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &ASlashCharacter::SetInputJemp); // �ϴ� ĳ���� ������ ������
		enhancedInputComponent->BindAction(ia_attack, ETriggerEvent::Started, this, &ASlashCharacter::Shoot);
		enhancedInputComponent->BindAction(ia_Fire, ETriggerEvent::Started, this, &ASlashCharacter::FireBullet);
	}



}
void ASlashCharacter::Move(FVector direction, float deltaTime)
{
	// direction�� �������� �̵��Ѵ�.
	// �̵� ���� ���: ����, �ӷ�, �ð� 
	FVector prevLocation = GetActorLocation(); // ������ġ���� �긮�� �����̼ǿ� ��´�. �������� ��������������.
	FVector nextLocation = prevLocation + direction * speed * deltaTime;
	SetActorLocation(nextLocation, true);  // bSweep�� �ٴ��� ���ٶ�� ������ �⺻�� flase�� �Ǿ�����. true�� �ٲ��ָ� �տ� �����ִ��� üũ�� �ϸ鼭 �̵���.

	//SetActorLocation(GetActorLocation() + direction * speed * deltaTime); ������ �ٿ����� �̷��� ��밡��.

}
void ASlashCharacter::SetInputDirection(const FInputActionValue& value)
{

	bool isPressed = value.Get<bool>();   // .�� ĳ��Ʈ�� �ǹ� , �����ڰ� ��� ������ ������ �� ��� �̷������� �ۼ�
	if (isPressed)
	{
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

		// ���� ������z�� ĳ����ĸ���ø������� �ΰ�
		playerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		playerAnim->bRunMotionOn = true;











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