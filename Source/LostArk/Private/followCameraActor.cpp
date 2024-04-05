// Fill out your copyright notice in the Description page of Project Settings.


#include "followCameraActor.h"
#include "GameFrameWork/SpringArmComponent.h" // 카메라 팔
#include "Camera/CameraComponent.h" // 카메라
#include "SlashCharacter.h" // 쫒아갈캐릭터 정보
#include "EngineUtils.h"





AfollowCameraActor::AfollowCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

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
	Super::BeginPlay();

	for (TActorIterator<ASlashCharacter> it(GetWorld()); it; ++it)
	{
		target = *it; // 클래스의 경우 초기화가 불가능하므로 이 for문으로 클래스를 찾아서 함수를 초기화시킴.
	}

	UE_LOG(LogTemp, Warning, TEXT("Camera Start!"));


}

void AfollowCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Camera tick!"));

	// 타겟의 위치를 구하고

	/*FVector ACtordir = target->GetActorLocation();
	FVector dir = ACtordir - GetActorLocation();
	dir.Normalize();
	SetActorLocation(GetActorLocation() + dir * 1000.0f * DeltaTime);*/

	
	 //그만큼 떨어진 위치에 배치
	if (target != nullptr)
	{ 
	FVector CamaraLoc = target-> GetActorLocation() + FVector(-500,100,800);  // 액터의 좌표를 타겟 loc로 받음

	SetActorLocation(FVector(CamaraLoc)); // 타겟의 로케이션값을 바꿈
	//UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), CamaraLoc.X, CamaraLoc.Y, CamaraLoc.Z);
	}




}
