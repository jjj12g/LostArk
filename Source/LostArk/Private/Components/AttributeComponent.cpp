// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;  // ƽ�� Ȱ��ȭ , ��Ȱ��ȭ ����


}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}




void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health -= FMath::Clamp(Health - Damage, 0.f, MaxHealth); // ü���� 0���� �۰ų� �ִ뺸�� ū������ ������������
	

}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth; // ü�°� �ƽ�ü���� ȣ��
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}