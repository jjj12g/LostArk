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

	//// �Ҹ�ã��
	//for (TActorIterator<AMybulletActor>itbullet(GetWorld()); itbullet; ++itbullet)
	//{
	//	target = *itbullet;
	//}

	

	SetLifeSpan(10.0f);
	// �Ҹ����� ������ Ȯ���ϴ¹�
	//UE_LOG(LogTemp, Warning, TEXT("Bullet: %s"), bullet == nullptr ? *FString("Null") : *FString("Has"));

}


void ABaseFloatingText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	damageText->SetText(FText::FromString(FString::SanitizeFloat(textingDamage())));

	if (GetOwner())
	{
		// ī�޶� �׻� �ٷκ����� ���� ���ֱ�.
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

