// Fill out your copyright notice in the Description page of Project Settings.


#include "slowhitActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>



AslowhitActor::AslowhitActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);

	// ����ƽ �޽� ������Ʈ�� �����ϰ� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� �����ϱ�
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

}


void AslowhitActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AslowhitActor::BeginOverlap);

}

void AslowhitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AslowhitActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("slowRain"));
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_slowPower, GetActorLocation(), GetActorRotation());

}




