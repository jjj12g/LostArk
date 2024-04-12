// Fill out your copyright notice in the Description page of Project Settings.


#include "MybulletActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AMybulletActor::AMybulletActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;

	bulletFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	SetRootComponent(bulletFX);
	
	Collisionsphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	Collisionsphere->SetupAttachment(bulletFX);
	Collisionsphere->SetCollisionProfileName(FName("Bullet preset"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
}

void AMybulletActor::BeginPlay()
{
	Super::BeginPlay();

	Collisionsphere->OnComponentBeginOverlap.AddDynamic(this, &AMybulletActor::BeginOverlap);

}

void AMybulletActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, impactparticle, GetActorLocation());
	BulletHit();

	AController* Playerc = GetInstigator()->GetController();
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, Playerc, this, DamageType);
	//AMybulletActor* 
	bullet = Cast<AMybulletActor>(OtherActor);

	if (bullet != nullptr && !GetWorld()->GetTimerManager().IsTimerActive(skillDelay))
	{
		GetWorld()->GetTimerManager().SetTimer(skillDelay, FTimerDelegate::CreateLambda([&]() {
			bullet-> Destroy();
			}), 3.0f, false);
	}

}



void AMybulletActor::BulletHit()
{
}


void AMybulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



