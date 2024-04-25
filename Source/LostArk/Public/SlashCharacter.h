

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h" // 에너미와 슬래시 캐릭터의 공통부모
#include "InputActionValue.h" // 이동
#include "CharacterTypes.h" // 캐릭터상태
#include "MybulletActor.h"
#include "SlashCharacter.generated.h" //generated파일은 맨밑에있는게 좋음



class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class UInputMappingContext;
class UInputAction;
class AMaactor;
class UHealthBarComponent; // hp 바
// 데미지 관련 슬래쉬캐릭터 h 와 cpp에 일단 주석으로 넣어둠






UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //공통부모로 변경
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
	
	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
	class USoundBase* QSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
	class USoundBase* WSound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
	class USoundBase* ESound;

	UPROPERTY(EditAnywhere, Category = "MySettings") // 사운드
	class USoundBase* RSound;

	// 체력 관련 ui 설정

	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UMainWidget> mainWidget_bp;

	UPROPERTY()
	class UMainWidget* mainWidget_inst; 

	
	void OnRepHealth();

	void UpdateHealth(float DamageAmount);

protected:
	virtual void BeginPlay() override;
	// 총알 발사 관련
	

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


	// 체력 값 변수 생성 및 설정
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

	// 캐릭터 컨트롤러 전역으로
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
	
	// 카메라 관련 변수 선언
	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UCameraComponent* cameraComp;
	
	// 캐릭터 움직임 구현
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //기본자료형이 아니니까 클래스선언을 해줘야함

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
	float speed = 500.0f; // 스피드

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* NI_space;

	USceneComponent* niagarasccomp;
	class UNiagaraComponent* niagaracomp;
	
	FVector targetPos; // 마우스로 찍은 타겟위치 전방선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	// 카메라 쉐이크 변수
	UPROPERTY(EditAnywhere, Category="MySettings|Variables")
	TSubclassOf<class UCameraShakeBase> PlayerHitShake_bp;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;} // 캐릭터애니메이션쪽에서 캐릭터상태를 가져가기위한세팅

	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value);
	
	void bcamerashake(); // 카메라 쉐이크 함수

	// 공격 모션 만들기
	UPROPERTY(EditAnywhere, Category = "Mysettings")
	UNiagaraSystem* NI_BASIC;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMaactor> Attackclass;

	UPROPERTY()
	AMaactor* Attack;

		//공격
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



	
private:   //나만 사용가능하다는 뜻 , 외부에서 호출할게 아니면 여기서 작성하는게 좋음

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // 캐릭터상태 평상시
		
		//UPROPERTY(EditAnyWhere, Category = "Damage)
		//float Damage = 20.f;				

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget; //hp 바

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitbrathMotage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMotages;

	void DeathMontage();

		//이동
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // 첫번째 매개변수는 값을 지정해주지 못함

	UFUNCTION()
	void ShiftStarted(const FInputActionValue& value); // 스페이스바 인풋 액션으로 쉬프트 함수 실행하기

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

	 //입력값을 받기위해 만든 함수 매개변수는 꼭 저 자료형으로 해줘야함.
	// 바인딩할거는 앞에 꼭 UFUNCTION()을 붙여주기. 이 함수가 있다는걸 언리얼 에디터에서 미리 알아야 하므로.
	UFUNCTION()
	void SetInputJemp(const FInputActionValue& value); // 점프 미완성	 
	
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



