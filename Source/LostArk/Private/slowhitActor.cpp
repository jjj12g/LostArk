// Fill out your copyright notice in the Description page of Project Settings.


#include "slowhitActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Enemy/Enemy.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "SlashCharacter.h"
#include "textnibox.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>


AslowhitActor::AslowhitActor()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);
	//boxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel1, ECollisionResponse::ECR_Overlap);
	


	// ����ƽ �޽� ������Ʈ�� �����ϰ� �ڽ� ������Ʈ�� �ڽ� ������Ʈ�� �����ϱ�
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	
	
		
}


void AslowhitActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AslowhitActor::BeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AslowhitActor::EndOverlap);

	 //���忡 �ִ� ���ξ��͸� ã�´�
	for (TActorIterator<Atextnibox>rain(GetWorld()); rain; ++rain)
	{
		rainActor = *rain; // Ŭ������ ������
	}
	

}

void AslowhitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (brainEnd)
	{ 
		rainTime += DeltaTime;
		if (rainTime < 6.0f)
		{
			bAttackrain = true;
		}
		else
		{
			
			rainTime = 0;
			Destroy();
		}

		if (bAttackrain)
		{ 
			if (rainAttackTime < 1.0f)
			{
				rainAttackTime += DeltaTime;
			}
			else
			{
				if (target != nullptr)
				{ 
					UGameplayStatics::ApplyDamage(target, 30, EnemyController, this, DamageType);
					rainAttackTime = 0;
					UE_LOG(LogTemp, Warning, TEXT("rain: %s"), *target->GetActorNameOrLabel());
				}
			}
		}
	}
}



void AslowhitActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_slowPower, GetActorLocation(), GetActorRotation());

	slowActor = Cast<AslowhitActor>(OtherActor);
	brainEnd = true;
	
	// �浹�� ���Ͱ� ATextnibox Ŭ������� �ڱ� �ڽ��� �ø��� �������� ����
	if(OtherActor->IsA<Atextnibox>())
	{
		boxComp->SetCollisionProfileName(FName("rainPreset"));
	}
	else if (OtherActor->IsA<ASlashCharacter>())
	{
		target = OtherActor;
	}
			
	

	// �״��� �������Ǹ� 6�ʵ��� 1�ʸ��� ���ö��� ������
	// 1�ʸ��� �������� ��
}

void AslowhitActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	target = nullptr;

}



	



	




