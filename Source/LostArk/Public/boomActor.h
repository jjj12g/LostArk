// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "boomActor.generated.h"

UCLASS()
class LOSTARK_API AboomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AboomActor();
	virtual void Tick(float DeltaTime) override;



protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp2;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;


	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY()
	class AActor* target;

	

	UPROPERTY()
	class AboomActor* boomActor;

	UPROPERTY()
	class AEnemy* enemy;


	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	
	

};
