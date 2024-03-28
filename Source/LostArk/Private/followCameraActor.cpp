// Fill out your copyright notice in the Description page of Project Settings.


#include "followCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // 카메라 팔
#include "Camera/CameraComponent.h" // 카메라
#include "SlashCharacter.h" // 쫒아갈캐릭터 정보






AfollowCameraActor::AfollowCameraActor()
{
	/*
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));  // 카메라암생성
	CameraBoom->SetupAttachment(GetRootComponent()); // 루트로 설정
	CameraBoom->SetUsingAbsoluteRotation(true); // 스프링암이 상위컴퍼넌트를 따르지않도록 설정
	CameraBoom->TargetArmLength = 800.0f; // 타겟과의 거리
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));

	

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false; // 플레이어 회전이 카메라 회전에 영향을 주지않게 설정
	*/
	
	//AfollowCameraActor* GetActorRotation(0.0f, -45.0f, 0.0f) = SetActorRotation();


}

void AfollowCameraActor::BeginPlay()
{
	

	



}

void AfollowCameraActor::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);

		// 타겟의 위치를 구하고
		ASlashCharacter* GetActorLocation();
		
		// 그만큼 떨어진 위치에 배치
		
			
		//AActor->SetActorLocation<ASlashCharacter>->GetComponentLocation(0, -60, 180));
		//AActor->SetActorRotation<ASlashCharacter>->GetComponentRotation(0, -45, 0));
		

	
}
