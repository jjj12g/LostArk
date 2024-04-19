// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFloatingText.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseFloatingText::ABaseFloatingText()
{
	PrimaryActorTick.bCanEverTick = true;

	damageAmount = 0.0f;

	damageText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DamageText"));
	damageText->SetupAttachment(GetRootComponent());
	damageText->SetText(FText::FromString("0")); 

}


void ABaseFloatingText::BeginPlay()
{
	Super::BeginPlay();
	
	damageText->SetText(FText::FromString(FString::SanitizeFloat(damageAmount)));

	SetLifeSpan(2.0f);
}


void ABaseFloatingText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetOwner())
	{
		damageText->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetOwner()->GetActorLocation()));
	}
}

