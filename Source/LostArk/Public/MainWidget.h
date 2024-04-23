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

		void SetOwnerPlayer(ASlashCharacter* player) { OwnerPlayer = player;}

protected:
	


	TWeakObjectPtr<ASlashCharacter> OwnerPlayer;

	UPROPERTY(meta = ( BindWidget ))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthLabel;


};
