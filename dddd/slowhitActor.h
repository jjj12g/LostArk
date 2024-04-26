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

	bool bAttackrain = false;
	bool brainEnd = false;
	float rainTime = 0;
	float rainAttackTime = 0;

	UPROPERTY()
	class AAIController* EnemyController;

	/*UPROPERTY()
	AActor* OtherActor;*/

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Mysettings")
	class AslowhitActor* slowActor;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class AEnemy* enemy;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class Atextnibox* rainActor;

	class AActor* target;
	
	

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
