// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "BaseFloatingText.generated.h"

UCLASS()
class LOSTARK_API ABaseFloatingText : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseFloatingText();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Text", meta = (ExposeOnSpawn = "true"))
	float damageAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TEXT")
	class UTextRenderComponent* damageText;



protected:
	virtual void BeginPlay() override;

};