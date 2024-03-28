// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" // ���ʹ̿� ������ ĳ������ ����θ�
#include "InputActionValue.h" // �̵�
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;


class UInputMappingContext;
class UInputAction;




UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //����θ�� ����
{
	GENERATED_BODY()






public:
	ASlashCharacter();
	



protected:
	virtual void BeginPlay() override;

	
	


	
public:
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ĳ���� ������ ����
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //�⺻�ڷ����� �ƴϴϱ� Ŭ���������� �������

	

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float speed = 500.0f; // ���ǵ�



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	


	private:   //���� ��밡���ϴٴ� �� , �ܺο��� ȣ���Ұ� �ƴϸ� ���⼭ �ۼ��ϴ°� ����

		


		//�̵�
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // ù��° �Ű������� ���� ���������� ����




	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value); //�Է°��� �ޱ����� ���� �Լ� �Ű������� �� �� �ڷ������� �������.
	// ���ε��ҰŴ� �տ� �� UFUNCTION()�� �ٿ��ֱ�. �� �Լ��� �ִٴ°� �𸮾� �����Ϳ��� �̸� �˾ƾ� �ϹǷ�.


	FVector targetPos;  // ���콺�� ���� Ÿ����ġ ���漱��
	

};
