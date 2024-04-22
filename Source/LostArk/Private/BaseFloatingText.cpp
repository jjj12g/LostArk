// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFloatingText.h"
#include "Kismet/KismetMathLibrary.h"
#include "MybulletActor.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

ABaseFloatingText::ABaseFloatingText()
{
	PrimaryActorTick.bCanEverTick = true;

	//damageAmount = 0.0f;

	damageText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DamageText"));
	damageText->SetupAttachment(GetRootComponent());
	//damageText->SetText(FText::FromString("0"));
	UE_LOG(LogTemp, Warning, TEXT("0"));



}



void ABaseFloatingText::BeginPlay()
{
	Super::BeginPlay();

	//bullet = GetOwner<AMybulletActor>();

	//// 불릿찾기
	//for (TActorIterator<AMybulletActor>itbullet(GetWorld()); itbullet; ++itbullet)
	//{
	//	target = *itbullet;
	//}

	

	SetLifeSpan(10.0f);
	// 불릿액터 널인지 확인하는법
	//UE_LOG(LogTemp, Warning, TEXT("Bullet: %s"), bullet == nullptr ? *FString("Null") : *FString("Has"));

}


void ABaseFloatingText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	damageText->SetText(FText::FromString(FString::SanitizeFloat(textingDamage())));

	if (GetOwner())
	{
		// 카메라 항상 바로보도록 설정 해주기.
		damageText->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetOwner()->GetActorLocation()));
		//UE_LOG(LogTemp, Warning, TEXT("tick1"));
	}
}

float ABaseFloatingText::textingDamage()
{

	if (bullet != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT(" test!!!!!! "));
		return bullet->BaseDamage;
	}
	return 0;
}

