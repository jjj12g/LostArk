// Fill out your copyright notice in the Description page of Project Settings.


#include "followCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // ī�޶� ��
#include "Camera/CameraComponent.h" // ī�޶�
#include "SlashCharacter.h" // �i�ư�ĳ���� ����
#include "EngineUtils.h"





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
	
	for (TActorIterator<ASlashCharacter> it(GetWorld()); it; ++it)
	{
		target = *it; // Ŭ������ ��� �ʱ�ȭ�� �Ұ����ϹǷ� �� for������ Ŭ������ ã�Ƽ� �Լ��� �ʱ�ȭ��Ŵ.
	}
	



}

void AfollowCameraActor::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);

		

		// Ÿ���� ��ġ�� ���ϰ�
		
		/*FVector ACtordir = target->GetActorLocation();
		FVector dir = ACtordir - GetActorLocation();
		dir.Normalize();
		SetActorLocation(GetActorLocation() + dir * 1000.0f * DeltaTime);*/


		// �׸�ŭ ������ ��ġ�� ��ġ
		//if (target != nullptr)
		//{ 
		FVector targetloc = target->GetActorLocation();  // ������ ��ǥ�� Ÿ�� loc�� ����
		
		 target->SetActorLocation(FVector(1,1,1), true); // Ÿ���� �����̼ǰ��� �ٲ�
		UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"),targetloc.X, targetloc.Y, targetloc.Z);
		//}


		

}
