// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tonaedoActor.generated.h"

UCLASS()
class LOSTARK_API AtonaedoActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AtonaedoActor();
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "MySettings") // ÆÄÆ¼Å¬
	class UParticleSystem* tonadoFX;


protected:
	virtual void BeginPlay() override;

	float tonaedoTime = 0;



};
