

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" // ���ʹ̿� ������ ĳ������ ����θ�
#include "InputActionValue.h" // �̵�
#include "CharacterTypes.h" // ĳ���ͻ���
#include "SlashCharacter.generated.h" //generated������ �ǹؿ��ִ°� ����

//class USpringArmComponent;
//class UCameraComponent;//
//
//class UInputMappingContext;
//class UInputAction;
// ������ ���� ������ĳ���� h �� cpp�� �ϴ� �ּ����� �־��

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

	// ī�޶� ���� ���� ����
	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class USpringArmComponent* springArmComp;

	// ĳ���� ������ ����
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //�⺻�ڷ����� �ƴϴϱ� Ŭ���������� �������

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_Jump; 

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float speed = 500.0f; // ���ǵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;} // ĳ���;ִϸ��̼��ʿ��� ĳ���ͻ��¸� �����������Ѽ���
		
	FVector targetPos;  // ���콺�� ���� Ÿ����ġ ���漱��

private:   //���� ��밡���ϴٴ� �� , �ܺο��� ȣ���Ұ� �ƴϸ� ���⼭ �ۼ��ϴ°� ����

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // ĳ���ͻ��� ����
			
	//UPROPERTY(EditAnyWhere, Category = "Damage)
	//float Damage = 20.f;	

	//�̵�
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // ù��° �Ű������� ���� ���������� ����

	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value); //�Է°��� �ޱ����� ���� �Լ� �Ű������� �� �� �ڷ������� �������.
	// ���ε��ҰŴ� �տ� �� UFUNCTION()�� �ٿ��ֱ�. �� �Լ��� �ִٴ°� �𸮾� �����Ϳ��� �̸� �˾ƾ� �ϹǷ�.
	UFUNCTION()
	void SetInputJump(const FInputActionValue& value); // ���� �̿ϼ�	

};



