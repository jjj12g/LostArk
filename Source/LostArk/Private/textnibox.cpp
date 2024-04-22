// Fill out your copyright notice in the Description page of Project Settings.


#include "textnibox.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "Enemy/Enemy.h"



// Sets default values
Atextnibox::Atextnibox()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetGenerateOverlapEvents(true);

	// 스태틱 메시 컴포넌트를 생성하고 박스 컴포넌트의 자식 컴포넌트로 지정하기
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	
}


void Atextnibox::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &Atextnibox::BeginOverlap);

	slowStart = GetActorLocation();
	loctarget = GetActorLocation() + FVector(0, 0, 700);
	downTarget = GetActorLocation() + FVector(0, 0, -800); 
	bslowUPTarget = true;
	
	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_slowRain, GetActorLocation(), GetActorRotation());

}

void Atextnibox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("111111111111111"));
	Destroy();

}


void Atextnibox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	slowUPTime += DeltaTime;
	

	// 테스트 느린비
	if (bslowUPTarget)
	{
		if (slowUPTime < 3.0f)
		{
			SetActorLocation(FMath::Lerp(slowStart, loctarget, slowUPTime * 0.3));
			//UE_LOG(LogTemp, Warning, TEXT("111111111111111"));
		}
		else
		{
			bslowDownTarget = true;
			slowUPTime = 0;
			//targetLoc = GetActorLocation() + GetActorForwardVector() * 1500;
			bslowUPTarget = false;
		}
	}


		if (bslowDownTarget)
		{
			slowDownTime += DeltaTime;
			if (slowDownTime < 10.0f)
			{
				SetActorLocation(FMath::Lerp(loctarget, downTarget, slowUPTime * 0.1));
				//UE_LOG(LogTemp, Warning, TEXT("222222222222"));
			}
			else
			{	
				slowDownTime = 0;
				bslowDownTarget = false;
			}
		}
	
}

