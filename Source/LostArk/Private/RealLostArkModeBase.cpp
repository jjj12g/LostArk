// Fill out your copyright notice in the Description page of Project Settings.


#include "RealLostArkModeBase.h"
#include "MainWidget.h"

void ARealLostArkModeBase::RespawnPlayer(AController* newplayer, APawn* previouspawn)
{
	//기존 폰에서 플레이어 컨트롤러를 떼어낸다.
		newplayer->UnPossess();

	//게임모드 베이스에 있는 재생성 함수를 실행한다.
	RestartPlayer(newplayer);

	// 기존의 폰을 제거한다.
	previouspawn->Destroy();
}

void ARealLostArkModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget_bp != nullptr)
	{
		mainWidget_inst = CreateWidget<UMainWidget>(GetWorld(), mainWidget_bp);

		if (mainWidget_inst)
		{
			mainWidget_inst->AddToViewport(0);
		}
	}
}
