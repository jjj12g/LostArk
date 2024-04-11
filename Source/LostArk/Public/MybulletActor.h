// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MybulletActor.generated.h"

UCLASS()
class LOSTARK_API AMybulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMybulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	class UNiagaraComponent* bulletFX;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	class USphereComponent* Collisionsphere;

	// 공격 날아가는 것 발사체 이동
	UPROPERTY(EditAnywhere, Category = "mysettings")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	float BaseDamage;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* impactparticle;

	virtual void BulletHit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
