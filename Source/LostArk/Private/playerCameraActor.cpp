// Fill out your copyright notice in the Description page of Project Settings.


#include "playerCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // 카메라 팔
#include "Camera/CameraComponent.h" // 카메라
#include "SlashCharacter.h" // 쫒아갈캐릭터 정보

AplayerCameraActor::AplayerCameraActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));  // 카메라암생성
	CameraBoom->SetupAttachment(GetRootComponent()); // 루트로 설정
	CameraBoom->SetUsingAbsoluteRotation(true); // 스프링암이 상위컴퍼넌트를 따르지않도록 설정
	CameraBoom->TargetArmLength = 800.0f; // 타겟과의 거리
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));



	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->bUsePawnControlRotation = false; // 플레이어 회전이 카메라 회전에 영향을 주지않게 설정


}


void AplayerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AplayerCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 플레이어의 좌표를 보고
	FVector dir = GetActorLocation();
	// 플레이어를 따라간다.


}

