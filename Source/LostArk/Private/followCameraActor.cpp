// Fill out your copyright notice in the Description page of Project Settings.


#include "followCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // ī�޶� ��
#include "Camera/CameraComponent.h" // ī�޶�
#include "SlashCharacter.h" // �i�ư�ĳ���� ����
#include "EngineUtils.h"





AfollowCameraActor::AfollowCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	/*
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));  // ī�޶�ϻ���
	CameraBoom->SetupAttachment(GetRootComponent()); // ��Ʈ�� ����
	CameraBoom->SetUsingAbsoluteRotation(true); // ���������� �������۳�Ʈ�� �������ʵ��� ����
	CameraBoom->TargetArmLength = 800.0f; // Ÿ�ٰ��� �Ÿ�
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));



	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false; // �÷��̾� ȸ���� ī�޶� ȸ���� ������ �����ʰ� ����
	*/

	//AfollowCameraActor* GetActorRotation(0.0f, -45.0f, 0.0f) = SetActorRotation();



}

void AfollowCameraActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASlashCharacter> it(GetWorld()); it; ++it)
	{
		target = *it; // Ŭ������ ��� �ʱ�ȭ�� �Ұ����ϹǷ� �� for������ Ŭ������ ã�Ƽ� �Լ��� �ʱ�ȭ��Ŵ.
	}

	UE_LOG(LogTemp, Warning, TEXT("Camera Start!"));


}

void AfollowCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Camera tick!"));

	// Ÿ���� ��ġ�� ���ϰ�

	/*FVector ACtordir = target->GetActorLocation();
	FVector dir = ACtordir - GetActorLocation();
	dir.Normalize();
	SetActorLocation(GetActorLocation() + dir * 1000.0f * DeltaTime);*/

	
	 //�׸�ŭ ������ ��ġ�� ��ġ
	if (target != nullptr)
	{ 
	FVector CamaraLoc = target-> GetActorLocation() + FVector(-500,100,800);  // ������ ��ǥ�� Ÿ�� loc�� ����

	SetActorLocation(FVector(CamaraLoc)); // Ÿ���� �����̼ǰ��� �ٲ�
	//UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), CamaraLoc.X, CamaraLoc.Y, CamaraLoc.Z);
	}




}
