// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h" // ü�¹� ����� ����


void UHealthBarComponent::SetHealthPercent(float Percent)
{
	// null ���� ���� Cast ( ĳ��Ʈ�ϴµ� �޸𸮸� ���� �Ҹ��ϹǷ�)
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->HealthBar) // �ｺ�������� ���̸� ���̻� if���� ã���ʰ� ����
	{
		HealthBarWidget->HealthBar->SetPercent(Percent); // ü�¹� ����� ����
	}
}
