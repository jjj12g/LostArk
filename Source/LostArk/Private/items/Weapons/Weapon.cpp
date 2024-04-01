// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapons/Weapon.h"
#include "SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h" // 다른물체랑 충돌




AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	// //플레이어에게는 부딪히지 않게 콜리전설정
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());


}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap); // 박스 트레이스를 하기위한 준비


}







void AWeapon::AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName)
{


}



void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


}



void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	{
		const FVector Start = BoxTraceStart->GetComponentLocation();
		const FVector End = BoxTraceEnd->GetComponentLocation();




		TArray<AActor*> ActorsToIgnere; // 플레이어를 공격안하기위한 배열
		ActorsToIgnere.Add(this); // 무기자체에 타격x 무시할 액터를 입력 매개변수로 전달 할 수 있음
		FHitResult BoxHit;
		UKismetSystemLibrary::BoxTraceSingle(
			this,
			Start,
			End,
			FVector(5.f, 5.f, 5.f),
			BoxTraceStart->GetComponentRotation(),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnere,
			EDrawDebugTrace::ForDuration,
			BoxHit,
			true  //자신을 무시
		);
		}
}





	// 무기에 히트되면 데미지 입히기

	/*
	if (BoxHit.GetActor())
	{
		
		if (HitInterface)
		{
			UGameplayStatics::ApplyDamage(
				BoxHit.GetActor(),
				Damage,
				GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
			);

			IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());

		CrateFields(BoxHit.ImpactPoint);

		




	}
	
}
*/