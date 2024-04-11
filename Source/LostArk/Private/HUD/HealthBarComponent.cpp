// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h" // 체력바 백분율 조절


void UHealthBarComponent::SetHealthPercent(float Percent)
{
	// null 값일 때만 Cast ( 캐스트하는데 메모리를 많이 소모하므로)
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	
	if (HealthBarWidget && HealthBarWidget->HealthBar) // 헬스바위젯이 널이면 더이상 if문을 찾지않게 설정
	{
		HealthBarWidget->HealthBar->SetPercent(Percent); // 체력바 백분율 조정
	}
}
