// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "Components/InputComponent.h"
#include "GameFrameWork/SpringArmComponent.h" // ī�޶� ��
#include "Camera/CameraComponent.h" // ī�޶�
#include "EnhancedInputComponent.h" // �̵�
#include "EnhancedInputSubsystems.h" // �̵�
#include "GameFramework/PlayerController.h" // �÷��̾� ��Ʈ�ѷ�
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "Components/CapsuleComponent.h"  // �÷��̾� ĸ�� ������Ʈ
#include "GameFramework/CharacterMovementComponent.h" //ĳ���� �̵�

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


		CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));  // ī�޶�ϻ���
		CameraBoom->SetupAttachment(GetRootComponent()); // ��Ʈ�� ����
		CameraBoom->SetUsingAbsoluteRotation(true); // ���������� �������۳�Ʈ�� �������ʵ��� ����
		CameraBoom->TargetArmLength = 800.0f; // Ÿ�ٰ��� �Ÿ�
		CameraBoom->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
		
		

		ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
		ViewCamera->SetupAttachment(CameraBoom);
		ViewCamera->bUsePawnControlRotation = false; // �÷��̾� ȸ���� ī�޶� ȸ���� ������ �����ʰ� ����

		
		

		
	

		
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
			//DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
			//UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);
			
		}

		//DrawDebugSphere(GetWorld(), mousePos, 1.0f, 5, FColor::Red, false, -1, 1, 1);

		// ���� ������z�� ĳ����ĸ���ø������� �ΰ�

		
		

		
		
		
		




	}

	}

