// Fill out your copyright notice in the Description page of Project Settings.


#include "items/item.h"
#include "DrawDebugHelpers.h" // 디버그 도우미(디버그 구등)
#include "SlashCharacter.h" // 디버그 기능 둔곳

#define THIRTY 30  //매크로기능

Aitem::Aitem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void Aitem::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message!"));

	}


	UWorld* World = GetWorld();  //우리가 있는 세계를 알도록 제공

	FVector Location = GetActorLocation();
	//DRAW_SPHERE(Location) //매크로로 세미콜론까지 복사됨

		//위가 매크로로 요약된코드
		/*if (World)  // if world와 if world포인터가 null인경우 검사실패
		{
			FVector Location = GetActorLocation();    //F백터유형의 로컬변수 생성, 액터상속함수사용초기화
			DrawDebugSphere(World, Location, 25.f, THIRTY, FColor::Red, false, 30.f);
		}
		*/




	
}


void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime); //델타타임이 부동소수점

	if (GEngine)
	{
		FString Name = GetName();  //문자열 값으로 문자열을 포맷하는법
		FString Message = FString::Printf(TEXT("Item Name: %s"), *Name); //F문자열 유형은 문자열초기화를 시켜줌 그래서 F문자열 개체를 만들 수 있음, 네임앞별은 포인터가아님C스타일 문자열을 제공하는것 F문자열 유형에 사용자지정기능이 있음을 의미  
		// C스타일 문자열은 단순히 문자배열이다.
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	}
	*/

}

