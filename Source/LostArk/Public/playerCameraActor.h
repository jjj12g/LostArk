// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "playerCameraActor.generated.h"

UCLASS()
class LOSTARK_API AplayerCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AplayerCameraActor();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	//Ä«¸Þ¶ó
		UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* ViewCamera;


};
