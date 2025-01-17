// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOSTARK_API IHitInterface
{
	GENERATED_BODY()

	
public:
	virtual void GetHit_Implementation(const FVector& ImpactPoint) = 0;



};
