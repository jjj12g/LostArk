


// Fill out your copyright notice in the Description page of Project Settings.


#include "RealLostArkModeBase.h"
#include "MainWidget.h"

void ARealLostArkModeBase::RespawnPlayer(AController* newplayer, APawn* previouspawn)
{
	//���� ������ �÷��̾� ��Ʈ�ѷ��� �����.
	newplayer->UnPossess();

	//���Ӹ�� ���̽��� �ִ� ����� �Լ��� �����Ѵ�.
	RestartPlayer(newplayer);

	// ������ ���� �����Ѵ�.
	previouspawn->Destroy();
}

void ARealLostArkModeBase::BeginPlay()
{
	Super::BeginPlay();

}

