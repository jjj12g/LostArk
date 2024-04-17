// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(VisibleAnywhere, Category = "Mysettings", meta = (BindWidget))
	class UProgressBar* PB_healthBar;

	void  SetHealthBar(float value, FLinearColor lowHealthColor);
	
};
