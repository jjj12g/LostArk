// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include "Maactor.generated.h"

UCLASS()
class LOSTARK_API AMaactor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMaactor();
	class ASlashCharacter* Playermouse;

	void PullTrigger();

	

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "mysettings")
	float Damage;

	void WeaponShoot();

	//UFUNCTION(BlueprintPure)
	//float calculateDamage(float Damage, float Mutiplier);

public:	
	
	virtual void Tick(float DeltaTime) override;
	

private:
	UPROPERTY(VisibleAnywhere,Category = "Mysettings")
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, Category = "Mysettings")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Mysettings")
	class UNiagaraSystem* NI_BASIC;

	UPROPERTY()
	class ASlashCharacter* player;

	APlayerController* GetController()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
};
