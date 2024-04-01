// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Weapons/Weapon.h"
#include "SlashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h" // �ٸ���ü�� �浹




AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	// //�÷��̾�Դ� �ε����� �ʰ� �ݸ�������
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

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap); // �ڽ� Ʈ���̽��� �ϱ����� �غ�


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




		TArray<AActor*> ActorsToIgnere; // �÷��̾ ���ݾ��ϱ����� �迭
		ActorsToIgnere.Add(this); // ������ü�� Ÿ��x ������ ���͸� �Է� �Ű������� ���� �� �� ����
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
			true  //�ڽ��� ����
		);
		}
}





	// ���⿡ ��Ʈ�Ǹ� ������ ������

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