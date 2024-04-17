// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealthBar(float value, FLinearColor lowHealthColor)
{
	PB_healthBar->SetPercent(value);
	//pb_healthBar->SetFillColorAndOpacity() 

	if (value < 0.2f)
	{
		PB_healthBar->SetFillColorAndOpacity(lowHealthColor);
	}
}
