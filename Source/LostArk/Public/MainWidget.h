// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SlashCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidgetBlueprint.h>

#include "MainWidget.generated.h"


UCLASS()
class LOSTARK_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

		//void SetOwnerPlayer(ASlashCharacter* player) { OwnerPlayer = player;}

		//void  SetHealthBar(float value, FLinearColor lowHealthColor);

public:


	void SetHealth(float CurrentHP, float MaxHP);

	void SetPower(float currentMp, float MaxMP);

	void SetHealthNum(float CurrentNum, float MaxHealthNum);

	void SetMPNum(float MPNum, float MaxMPNum);

	void SetPortionNUMS(float CurrentHP, float CurrentNum, float PortionNumber);

	void SetCooltime(float skillCollTime, float skillCollTimer);

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


	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TArray<float> percents = { 0.5f, 0.7f };*/

	
	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* Fire;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* ice;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* Energy;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* Mete;


protected:
	
	TWeakObjectPtr<ASlashCharacter> OwnerPlayer;

	/*UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* MaxHealthLabel;*/

	


};
