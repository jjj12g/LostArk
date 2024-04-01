// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;  // 틱을 활성화 , 비활성화 가능


}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}




void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health -= FMath::Clamp(Health - Damage, 0.f, MaxHealth); // 체력을 0보다 작거나 최대보다 큰것으로 설정하지않음
	

}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth; // 체력과 맥스체력을 호출
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}