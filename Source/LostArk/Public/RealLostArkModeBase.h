// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RealLostArkModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API ARealLostArkModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UMainWidget> mainWidget_bp;

	UPROPERTY()
	class UMainWidget* mainWidget_inst;


	void RespawnPlayer(AController* newplayer, APawn* previouspawn);

protected:
	virtual void BeginPlay() override;
	
};
