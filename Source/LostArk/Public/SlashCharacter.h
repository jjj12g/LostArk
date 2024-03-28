// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" // 에너미와 슬래시 캐릭터의 공통부모
#include "InputActionValue.h" // 이동
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;


class UInputMappingContext;
class UInputAction;




UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //공통부모로 변경
{
	GENERATED_BODY()






public:
	ASlashCharacter();
	



protected:
	virtual void BeginPlay() override;

	
	


	
public:
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터 움직임 구현
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //기본자료형이 아니니까 클래스선언을 해줘야함

	

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float speed = 500.0f; // 스피드



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	


	private:   //나만 사용가능하다는 뜻 , 외부에서 호출할게 아니면 여기서 작성하는게 좋음

		


		//이동
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // 첫번째 매개변수는 값을 지정해주지 못함




	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value); //입력값을 받기위해 만든 함수 매개변수는 꼭 저 자료형으로 해줘야함.
	// 바인딩할거는 앞에 꼭 UFUNCTION()을 붙여주기. 이 함수가 있다는걸 언리얼 에디터에서 미리 알아야 하므로.


	FVector targetPos;  // 마우스로 찍은 타겟위치 전방선언
	

};
