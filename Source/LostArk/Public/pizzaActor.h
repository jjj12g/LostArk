// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "pizzaActor.generated.h"

UCLASS()
class LOSTARK_API ApizzaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ApizzaActor();

	UPROPERTY(EditAnywhere, Category = "MySettings") // ÆÄÆ¼Å¬
	class UNiagaraSystem* NI_Pizza;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
