// Fill out your copyright notice in the Description page of Project Settings.


#include "MybulletActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GamePlayStatics.h"


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
	BulletHit();
	AController* Playerc = GetInstigator()->GetController();
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, Playerc, this, DamageType);
	AMybulletActor* Destroy(this);
}



void AMybulletActor::BulletHit()
{
}


void AMybulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
