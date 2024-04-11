// Fill out your copyright notice in the Description page of Project Settings.


#include "items/item.h"
#include "DrawDebugHelpers.h" // ����� �����(����� ����)
#include "SlashCharacter.h" // ����� ��� �а�

#define THIRTY 30  //��ũ�α��

Aitem::Aitem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void Aitem::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin Play called!"));


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("Item OnScreen Message!"));

	}


	UWorld* World = GetWorld();  //�츮�� �ִ� ���踦 �˵��� ����

	FVector Location = GetActorLocation();
	//DRAW_SPHERE(Location) //��ũ�η� �����ݷб��� �����

		//���� ��ũ�η� �����ڵ�
		/*if (World)  // if world�� if world�����Ͱ� null�ΰ�� �˻����
		{
			FVector Location = GetActorLocation();    //F���������� ���ú��� ����, ���ͻ���Լ�����ʱ�ȭ
			DrawDebugSphere(World, Location, 25.f, THIRTY, FColor::Red, false, 30.f);
		}
		*/




	
}


void Aitem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime); //��ŸŸ���� �ε��Ҽ���

	if (GEngine)
	{
		FString Name = GetName();  //���ڿ� ������ ���ڿ��� �����ϴ¹�
		FString Message = FString::Printf(TEXT("Item Name: %s"), *Name); //F���ڿ� ������ ���ڿ��ʱ�ȭ�� ������ �׷��� F���ڿ� ��ü�� ���� �� ����, ���Ӿպ��� �����Ͱ��ƴ�C��Ÿ�� ���ڿ��� �����ϴ°� F���ڿ� ������ �������������� ������ �ǹ�  
		// C��Ÿ�� ���ڿ��� �ܼ��� ���ڹ迭�̴�.
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	}
	*/

}

