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
	// Sets default values for this actor's properties
	AMaactor();
	class ASlashCharacter* Playermouse;

	void PullTrigger();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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
