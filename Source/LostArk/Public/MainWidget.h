// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashCharacter.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOSTARK_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

		//void SetOwnerPlayer(ASlashCharacter* player) { OwnerPlayer = player;}

		void  SetHealthBar(float value, FLinearColor lowHealthColor);

public:


	void SetHealth(float CurrentHP, float MaxHP);

	void SetPower(float currentMp, float MaxMP);

	void SetHealthNum(float CurrentNum, float MaxHealthNum);

	void SetMPNum(float MPNum, float MaxMPNum);

	void SetPortionNUMS(float CurrentHP, float CurrentNum, float PortionNumber);

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* MPBar;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* MaxHealthLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* CurrentMPLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* MaxMPLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UImage* Portion;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* PortionNum;


protected:
	
	TWeakObjectPtr<ASlashCharacter> OwnerPlayer;

	/*UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* MaxHealthLabel;*/

	


};
