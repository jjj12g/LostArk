

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
class UHealthBarComponent; // hp ��
// ������ ���� ������ĳ���� h �� cpp�� �ϴ� �ּ����� �־��






UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //����θ�� ����
{
	GENERATED_BODY()






public:
	ASlashCharacter();
	
	AActor* ShootBullet();
	AActor* ShootBullet2();
	AActor* ShootBullet3();
	AActor* ShootBullet4();
	AActor* ShootBullet5();
	AActor* ShootBullet6();
	AActor* ShootBullet7();
	AActor* ShootBullet8();
	AActor* ShootBullet9();

	
	bool camrashake = false;
	bool bskillCollTime = false;
	
	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
	class USoundBase* QSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
	class USoundBase* WSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
	class USoundBase* ESound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // ����
	class USoundBase* RSound;

	// ü�� ���� ui ����

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UMainWidget> mainWidget_bp;

	UPROPERTY()
	class UMainWidget* mainWidget_inst; 

	
	void OnRepHealth();

	void UpdateHealth(float DamageAmount);

protected:
	virtual void BeginPlay() override;
	// �Ѿ� �߻� ����
	

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn2;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn4;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn5;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn6;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn7;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn8;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMybulletActor> bullettospawn9;


	// ü�� �� ���� ���� �� ����
	UPROPERTY(EditAnywhere)
	int32 MaxHP = 1000;

	UPROPERTY(EditAnywhere)
	int32 MaxHealthNum = 1000;
	UPROPERTY(EditAnywhere)
	int32 MaxMPNum = 3000;
	UPROPERTY(EditAnywhere)
	int MaxMP = 3000;

	UPROPERTY(EditAnywhere)
	int PortionNumber = 10;

	float manaTime = 0;

	FORCEINLINE int32 GetcurrentHP() { return currentHP; };
	
	
	UPROPERTY()
	float skillCollTimer = 0.0f;

	UPROPERTY()
	class UHealthBarWidget* PlayerWidget;


	class UMainWidget* mainHPWget;

	// ĳ���� ��Ʈ�ѷ� ��������
	UPROPERTY()
	class APlayerController* pc;

	
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


	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class USpringArmComponent* springArmComp;
	
	// ī�޶� ���� ���� ����
	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UCameraComponent* cameraComp;
	
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

	// ī�޶� ����ũ ����
	UPROPERTY(EditAnywhere, Category="MySettings|Variables")
	TSubclassOf<class UCameraShakeBase> PlayerHitShake_bp;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;} // ĳ���;ִϸ��̼��ʿ��� ĳ���ͻ��¸� �����������Ѽ���

	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value);
	
	void bcamerashake(); // ī�޶� ����ũ �Լ�

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

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* sky_montage;

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* straight_montage;

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* basic_montage;

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* damaged_montage;

	UPROPERTY(EditAnywhere, Category="MySettings|Animations")
	class UAnimMontage* died_montage;

	bool bKeyPressed;
	bool bAttackEnabled;
	bool bPlayerIsAttacking = false;



	
private:   //���� ��밡���ϴٴ� �� , �ܺο��� ȣ���Ұ� �ƴϸ� ���⼭ �ۼ��ϴ°� ����

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // ĳ���ͻ��� ����
		
		//UPROPERTY(EditAnyWhere, Category = "Damage)
		//float Damage = 20.f;				

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp ��

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitbrathMotage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMotages;

	void DeathMontage();

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
	
	void hitreact();

	UPROPERTY()
	class UPlayerAnimInstance* playerAnim;

	UPROPERTY()
	bool bPlayerIsInvisible=false;

	
	FVector CachedDestination;
	
	int32 currentHP = 0;
	int32 currentMP = 0;
	int32 CurrentNum = 0;
	int32 MPNum = 0;

	bool q;
	bool w;
	bool e;
	bool r;
};



