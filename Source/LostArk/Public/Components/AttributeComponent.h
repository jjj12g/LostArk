// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTARK_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;


private:
	
	// 체력
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	//최대체력
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

public:
	void ReceiveDamage(float Damage);  // 데미지 함수
	float GetHealthPercent();
	bool IsAlive(); // 살아있는지 확인

};
