// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

//void UMainWidget::SetHealthBar(float value, FLinearColor lowHealthColor)
//{
//
//
//	HealthBar->SetPercent(value);
//	//pb_healthBar->SetFillColorAndOpacity() 
//
//	if (value < 0.2f)
//	{
//		HealthBar->SetFillColorAndOpacity(lowHealthColor);
//	}
//
//	/*if(CurrentHealthLabel!=nullptr)
//	{ 
//		CurrentHealthLabel->Text = FText::FromString("5000");
//	}*/
//}

void UMainWidget::SetHealth(float CurrentHP, float MaxHP)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UMainWidget::SetPower(float currentMp, float MaxMP)
{
	if (MPBar)
	{
		MPBar->SetPercent(currentMp / MaxMP);
	}
}

void UMainWidget::SetHealthNum(float CurrentNum, float MaxHealthNum)
{
	if (CurrentHealthLabel)
	{
		CurrentHealthLabel->SetText(FText::FromString(FString::SanitizeFloat(CurrentNum)));
	}
}

void UMainWidget::SetMPNum(float MPNum, float MaxMPNum)
{
	if (CurrentMPLabel)
	{
		CurrentMPLabel->SetText(FText::FromString(FString::SanitizeFloat(MPNum)));
	}
}

void UMainWidget::SetPortionNUMS(float CurrentHP, float CurrentNum, float PortionNumber)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHP);
	}

	if (CurrentHealthLabel)
	{
		CurrentHealthLabel->SetText(FText::FromString(FString::SanitizeFloat(CurrentNum)));
	}

	if (PortionNum)
	{
		PortionNum->SetText(FText::FromString(FString::SanitizeFloat(PortionNumber--)));
	}
}

void UMainWidget::SetCooltime(float skillCollTime, float skillCollTimer)
{
	if (Fire)
	{
		Fire->SetPercent(skillCollTime / skillCollTimer);
	}
}
