// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "items/item.h"
#include "Weapon.generated.h"



class USoundBase;
class UBoxComponent;



/**
 * 
 */
UCLASS()
class LOSTARK_API AWeapon : public Aitem
{
	GENERATED_BODY()


public:
	AWeapon();


	void AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName);   // 소켓에 붙임

protected:
	virtual void BeginPlay() override;



	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);


	//virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
		
		// 사운드
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound; 

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	
};
