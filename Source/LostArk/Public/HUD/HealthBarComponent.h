// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()


public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()  // 체력바를 처음 null로 초기화
	class UHealthBar* HealthBarWidget;

	
};
