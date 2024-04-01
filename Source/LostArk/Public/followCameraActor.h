// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "followCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API AfollowCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:

	AfollowCameraActor();

	

	
	class ASlashCharacter* target;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;



private:
	//Ä«¸Þ¶ó
		//UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		//class USpringArmComponent* CameraBoom;

		//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		//class UCameraComponent* ViewCamera;


	
	
};
