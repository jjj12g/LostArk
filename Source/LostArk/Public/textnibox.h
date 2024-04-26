// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "textnibox.generated.h"

UCLASS()
class LOSTARK_API Atextnibox : public AActor
{
	GENERATED_BODY()
	
public:	
	Atextnibox();

	virtual void Tick(float DeltaTime) override;

	bool bslowUPTarget = false;
	bool bslowDownTarget = false;

	UPROPERTY()
	class UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_slowRain;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxsComp;
	


protected:
	virtual void BeginPlay() override;


	

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AEnemy* enemy;
	

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float slowUPTime = 0;
	float slowDownTime = 0;
	
	FVector slowStart;
	FVector loctarget;
	FVector downTarget;
	

};
