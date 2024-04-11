// Fill out your copyright notice in the Description page of Project Settings.


#include "playerCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // ī�޶� ��
#include "Camera/CameraComponent.h" // ī�޶�
#include "SlashCharacter.h" // �i�ư�ĳ���� ����

AplayerCameraActor::AplayerCameraActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));  // ī�޶�ϻ���
	CameraBoom->SetupAttachment(GetRootComponent()); // ��Ʈ�� ����
	CameraBoom->SetUsingAbsoluteRotation(true); // ���������� �������۳�Ʈ�� �������ʵ��� ����
	CameraBoom->TargetArmLength = 800.0f; // Ÿ�ٰ��� �Ÿ�
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));



	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false; // �÷��̾� ȸ���� ī�޶� ȸ���� ������ �����ʰ� ����


}


void AplayerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AplayerCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// �÷��̾��� ��ǥ�� ����
	FVector dir = GetActorLocation();
	// �÷��̾ ���󰣴�.


}

