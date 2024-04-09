// Fill out your copyright notice in the Description page of Project Settings.


#include "Maactor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>

#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "SlashCharacter.h"
#include "DrawDebugHelpers.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

// Sets default values
AMaactor::AMaactor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AMaactor::PullTrigger()
{
	

	FVector mouse = player->targetPos;
	FVector toward = mouse - player->GetActorLocation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_BASIC, player->GetActorLocation(), toward.Rotation());
	DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Red, true);
}

// Called when the game starts or when spawned
void AMaactor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ASlashCharacter> it(GetWorld()); it; ++it)
	{
		player = *it;
	}
}

void AMaactor::WeaponShoot()
{
}
/*
float AMaactor::calculateDamage(float Damage, float Mutiplier)
{
	return Damage* Mutiplier;
}*/

// Called every frame
void AMaactor::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
}

