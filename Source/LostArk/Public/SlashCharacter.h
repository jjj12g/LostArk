

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

// 데미지 관련 슬래쉬캐릭터 h 와 cpp에 일단 주석으로 넣어둠






UCLASS()
class LOSTARK_API ASlashCharacter : public ABaseCharacter //공통부모로 변경
{
	GENERATED_BODY()






public:
	ASlashCharacter();
	



protected:
	virtual void BeginPlay() override;
	// 총알 발사 관련
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
	// 캐릭터 움직임 구현
	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UInputMappingContext* imc_myMapping;

	UPROPERTY(EditAnywhere, Category="MySettings")
	class UInputAction* ia_move; //기본자료형이 아니니까 클래스선언을 해줘야함

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
	float speed = 500.0f; // 스피드
	
	FVector targetPos; // 마우스로 찍은 타겟위치 전방선언

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool inputDir;

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;} // 캐릭터애니메이션쪽에서 캐릭터상태를 가져가기위한세팅

	UFUNCTION() 
	void SetInputDirection(const FInputActionValue& value);	

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
	
private:   //나만 사용가능하다는 뜻 , 외부에서 호출할게 아니면 여기서 작성하는게 좋음

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped; // 캐릭터상태 평상시


		
		//UPROPERTY(EditAnyWhere, Category = "Damage)
		//float Damage = 20.f;

	
		

		

		//이동
	UFUNCTION(BlueprintCallable)
	void Move(FVector direction, float deltaTime);    // 첫번째 매개변수는 값을 지정해주지 못함

<<<<<<< HEAD
	 //입력값을 받기위해 만든 함수 매개변수는 꼭 저 자료형으로 해줘야함.
	// 바인딩할거는 앞에 꼭 UFUNCTION()을 붙여주기. 이 함수가 있다는걸 언리얼 에디터에서 미리 알아야 하므로.
	UFUNCTION()
	void SetInputJump(const FInputActionValue& value); // 점프 미완성	 
=======







	 //입력값을 받기위해 만든 함수 매개변수는 꼭 저 자료형으로 해줘야함.
	// 바인딩할거는 앞에 꼭 UFUNCTION()을 붙여주기. 이 함수가 있다는걸 언리얼 에디터에서 미리 알아야 하므로.
	UFUNCTION()
	void SetInputJemp(const FInputActionValue& value); // 점프 미완성
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



