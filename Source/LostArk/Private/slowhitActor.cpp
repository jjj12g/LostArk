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
	


	// 스태틱 메시 컴포넌트를 생성하고 박스 컴포넌트의 자식 컴포넌트로 지정하기
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	
	
		
}


void AslowhitActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AslowhitActor::BeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AslowhitActor::EndOverlap);

	 //월드에 있는 레인액터를 찾는다
	for (TActorIterator<Atextnibox>rain(GetWorld()); rain; ++rain)
	{
		rainActor = *rain; // 클래스의 다형성
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
	
	// 충돌한 액터가 ATextnibox 클래스라면 자기 자신의 컬리전 프로필을 변경
	if(OtherActor->IsA<Atextnibox>())
	{
		boxComp->SetCollisionProfileName(FName("rainPreset"));
	}
	else if (OtherActor->IsA<ASlashCharacter>())
	{
		target = OtherActor;
	}
			
	

	// 그다음 오버랩되면 6초동안 1초마다 어플라이 데미지
	// 1초마다 데미지를 줌
}

void AslowhitActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	target = nullptr;

}



	



	




