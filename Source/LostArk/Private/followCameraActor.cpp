// Fill out your copyright notice in the Description page of Project Settings.


#include "followCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // ī�޶� ��
#include "Camera/CameraComponent.h" // ī�޶�
#include "SlashCharacter.h" // �i�ư�ĳ���� ����






AfollowCameraActor::AfollowCameraActor()
{
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
	

	



}

void AfollowCameraActor::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);

		// Ÿ���� ��ġ�� ���ϰ�
		ASlashCharacter* GetActorLocation();
		
		// �׸�ŭ ������ ��ġ�� ��ġ
		
			
		//AActor->SetActorLocation<ASlashCharacter>->GetComponentLocation(0, -60, 180));
		//AActor->SetActorRotation<ASlashCharacter>->GetComponentRotation(0, -45, 0));
		

	
}
