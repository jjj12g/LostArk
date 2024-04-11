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
	
	// ü��
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	//�ִ�ü��
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

public:
	void ReceiveDamage(float Damage);  // ������ �Լ�
	float GetHealthPercent();
	bool IsAlive(); // ����ִ��� Ȯ��

};
