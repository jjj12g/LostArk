// Fill out your copyright notice in the Description page of Project Settings.


#include "tonaedoActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AtonaedoActor::AtonaedoActor()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AtonaedoActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), tonadoFX, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, FVector(7.0f)); //제로 로테이터(회전하지않음)

	SetLifeSpan(5.0f);

}


void AtonaedoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	tonaedoTime += DeltaTime;
		if (tonaedoTime < 4.0f)
		{
			Destroy();
		}


}

