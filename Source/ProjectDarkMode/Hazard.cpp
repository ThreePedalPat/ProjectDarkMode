// Fill out your copyright notice in the Description page of Project Settings.

#include "Hazard.h"
#include "PlayerCharacter.h"
#include "StatComponent.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));

}

// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AHazard::OnEnter);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AHazard::OnExit);
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHazard::OnEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		UStatComponent* pStats = player->GetComponentByClass<UStatComponent>();
		pStats->TakeDamage(hazardDamage);
		if (shouldTriggerRespawn)
		{
			OtherActor->SetActorLocation(player->LastCheckpoint);
		}
		if (shouldSetOnFire)
		{
			pStats->SetOnFire();
		}
		if (shouldLaunch)
		{
			FVector Dir = player->GetActorForwardVector();
			player->LaunchCharacter(FVector(-Dir.X * 200, -Dir.Y * 200, 700), true, true);
		}
	}
}

void AHazard::OnExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (player)
	{
		UStatComponent* pStats = player->GetComponentByClass<UStatComponent>();
	}
}

