

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
	
	AActor* ShootBullet();
	AActor* ShootBullet2();


protected:
	virtual void BeginPlay() override;
	// �Ѿ� �߻� ����
	

	

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn2;

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


	class ASlashCharacter* PlayerCharacter;

	bool CanFire = true;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	float TimeBetweenFire = 0.2f;

	UFUNCTION()
	void SetCanFire(bool value);

	FRotator ShootRot;
	FRotator Movement;
	
	

	
public:
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MySettings|Components")
	class UStaticMeshComponent* staffMeshComp;

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

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_attack;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_shift;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_q;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_w;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_e;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_r;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_a;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_s;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_d;

	UPROPERTY(EditAnywhere, Category="MySettings\Skills")
	class UInputAction* ia_f;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_Fire;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float speed = 500.0f; // ���ǵ�

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* NI_space;

	USceneComponent* niagarasccomp;
	class UNiagaraComponent* niagaracomp;
	
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

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* fencing_montage;

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* hitground_montage;

	UPROPERTY()
	bool bPlayerIsAttacking=false;

	
private:   //���� ��밡���ϴٴ� �� , �ܺο��� ȣ���Ұ� �ƴϸ� ���⼭ �ۼ��ϴ°� ����

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // ĳ���ͻ��� ����
		
		//UPROPERTY(EditAnyWhere, Category = "Damage)
		//float Damage = 20.f;				

		//�̵�
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // ù��° �Ű������� ���� ���������� ����

	UFUNCTION()
	void ShiftStarted(const FInputActionValue& value); // �����̽��� ��ǲ �׼����� ����Ʈ �Լ� �����ϱ�

	UFUNCTION()
	void Q(const FInputActionValue& value);

	UFUNCTION()
	void W(const FInputActionValue& value);

	UFUNCTION()
	void FireBullet2(const FInputActionValue& value);

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

	 //�Է°��� �ޱ����� ���� �Լ� �Ű������� �� �� �ڷ������� �������.
	// ���ε��ҰŴ� �տ� �� UFUNCTION()�� �ٿ��ֱ�. �� �Լ��� �ִٴ°� �𸮾� �����Ϳ��� �̸� �˾ƾ� �ϹǷ�.
	UFUNCTION()
	void SetInputJemp(const FInputActionValue& value); // ���� �̿ϼ�	 
	
	UPROPERTY()
	class UPlayerAnimInstance* playerAnim;

	UPROPERTY()
	bool bPlayerIsInvisible=false;

	
};



