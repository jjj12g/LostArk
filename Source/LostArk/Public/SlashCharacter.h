

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" // ���ʹ̿� ������ ĳ������ ����θ�
#include "InputActionValue.h" // �̵�
#include "CharacterTypes.h" // ĳ���ͻ���
#include "MybulletActor.h"
#include "SlashCharacter.generated.h" //generated������ �ǹؿ��ִ°� ����



class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class AMaactor;

// ������ ���� ������ĳ���� h �� cpp�� �ϴ� �ּ����� �־��






UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //����θ�� ����
{
	GENERATED_BODY()






public:
	ASlashCharacter();
	



protected:
	virtual void BeginPlay() override;
	// �Ѿ� �߻� ����
	AActor* ShootBullet();

	

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn;

	UPROPERTY(EditAnywhere)
	float HP = 50;

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	)override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SpawnLocation;
	
	void FireBullet(const FInputActionValue& value);
	


	
public:
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UStaticMeshComponent* staffMeshComp;

=======
>>>>>>> parent of ab17368 (jj)
=======
>>>>>>> parent of ab17368 (jj)
=======
>>>>>>> parent of ab17368 (jj)
	// ĳ���� ������ ����
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //�⺻�ڷ����� �ƴϴϱ� Ŭ���������� �������

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_Jump; 

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_attack;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputAction* ia_Fire;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_Q;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_W;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_E;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_R;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_A;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_S;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_D;

	UPROPERTY(EditAnywhere, Category = "MySettings\Skills")
	class UInputAction* ia_F;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float speed = 500.0f; // ���ǵ�
	
	FVector targetPos; // ���콺�� ���� Ÿ����ġ ���漱��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;} // ĳ���;ִϸ��̼��ʿ��� ĳ���ͻ��¸� �����������Ѽ���

	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value);	

	// ���� ��� �����
	UPROPERTY(EditAnywhere, Category = "Mysettings")
	UNiagaraSystem* NI_BASIC;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMaactor> Attackclass;

	UPROPERTY()
	AMaactor* Attack;

	//����
	UFUNCTION()
	void Shoot(const FInputActionValue& value);
	
private:   //���� ��밡���ϴٴ� �� , �ܺο��� ȣ���Ұ� �ƴϸ� ���⼭ �ۼ��ϴ°� ����

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // ĳ���ͻ��� ����


		
		//UPROPERTY(EditAnyWhere, Category = "Damage)
		//float Damage = 20.f;

	
		

		

		//�̵�
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // ù��° �Ű������� ���� ���������� ����

<<<<<<< HEAD
	 //�Է°��� �ޱ����� ���� �Լ� �Ű������� �� �� �ڷ������� �������.
	// ���ε��ҰŴ� �տ� �� UFUNCTION()�� �ٿ��ֱ�. �� �Լ��� �ִٴ°� �𸮾� �����Ϳ��� �̸� �˾ƾ� �ϹǷ�.
	UFUNCTION()
	void SetInputJump(const FInputActionValue& value); // ���� �̿ϼ�	 
=======







	 //�Է°��� �ޱ����� ���� �Լ� �Ű������� �� �� �ڷ������� �������.
	// ���ε��ҰŴ� �տ� �� UFUNCTION()�� �ٿ��ֱ�. �� �Լ��� �ִٴ°� �𸮾� �����Ϳ��� �̸� �˾ƾ� �ϹǷ�.
	UFUNCTION()
	void SetInputJemp(const FInputActionValue& value); // ���� �̿ϼ�
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======

	 
	
>>>>>>> parent of ab17368 (jj)

	 
	
>>>>>>> parent of ab17368 (jj)

	 
>>>>>>> parent of ab17368 (jj)
	

	UFUNCTION()
	void Q(const FInputActionValue& value);

	UFUNCTION()
	void W(const FInputActionValue& value);

	UFUNCTION()
	void E(const FInputActionValue& value);

	UFUNCTION()
	void R(const FInputActionValue& value);

	UFUNCTION()
	void A(const FInputActionValue& value);

	UFUNCTION()
	void S(const FInputActionValue& value);

	UFUNCTION()
	void D(const FInputActionValue& value);

	UFUNCTION()
	void F(const FInputActionValue& value);
};



