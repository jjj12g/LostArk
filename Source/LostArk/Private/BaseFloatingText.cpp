// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFloatingText.h"
#include "Kismet/KismetMathLibrary.h"


ABaseFloatingText::ABaseFloatingText()
{
	PrimaryActorTick.bCanEverTick = true;

	damageAmount = 0.0f;

	damageText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DamageText"));
	damageText->SetupAttachment(GetRootComponent());
	damageText->SetText(FText::FromString("0"));
	UE_LOG(LogTemp, Warning, TEXT("0"));



}

float ABaseFloatingText::textingDamage()
{
	if (bullet_bp)
	{
		UE_LOG(LogTemp, Warning, TEXT(" test!!!!!! "));

		//damageAmount = bullet_bp-> BaseDamage;

	}

	return damageAmount;
}

void ABaseFloatingText::BeginPlay()
{
	Super::BeginPlay();


	textingDamage();
	damageText->SetText(FText::FromString(FString::SanitizeFloat(damageAmount)));

	SetLifeSpan(10.0f);
	UE_LOG(LogTemp, Warning, TEXT("beginpaly"));


}


void ABaseFloatingText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (GetOwner())
	{
		damageText->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetOwner()->GetActorLocation()));
		UE_LOG(LogTemp, Warning, TEXT("tick1"));
	}
}

