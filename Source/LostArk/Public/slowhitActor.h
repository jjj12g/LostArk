// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "slowhitActor.generated.h"

UCLASS()
class LOSTARK_API AslowhitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AslowhitActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UNiagaraComponent* NiagaraComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_slowPower;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AEnemy* enemy;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
