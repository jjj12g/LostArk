// Fill out your copyright notice in the Description page of Project Settings.


#include "boomActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include"Enemy/Enemy.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include "SlashCharacter.h"

// Sets default values
AboomActor::AboomActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component3"));
	SetRootComponent(boxComp3);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetGenerateOverlapEvents(true);
	
	//boxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Overlap);


	boxComp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component2"));
	boxComp2->SetupAttachment(RootComponent);
	boxComp2->SetGenerateOverlapEvents(true);




}


void AboomActor::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AboomActor::BeginOverlap);
	

}

void AboomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	boobTime += DeltaTime;
	if (boobTime < 1.8f)
	{
	}
	else
	{
		boobTime = 0;
		Destroy();
	}

	if (bPizzaboom)
	{
		//UE_LOG(LogTemp, Warning, TEXT("boommmmmmmmmmmmmmm"));
		UGameplayStatics::ApplyDamage(target, 500, EnemyController, this, DamageType);
		bPizzaboom = false;
	}
	
}



void AboomActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	boomActor = Cast<AboomActor>(OtherActor);
	if (OtherActor->IsA<ASlashCharacter>())
	{
		target = OtherActor;
		bPizzaboom = true;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("boom"));
	
	
}

