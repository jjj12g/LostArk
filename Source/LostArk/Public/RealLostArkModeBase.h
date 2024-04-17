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

	void RespawnPlayer(AController* newplayer, APawn* previouspawn);
	
};
