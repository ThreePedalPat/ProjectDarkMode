// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectablePickup.h"

// Sets default values
ACollectablePickup::ACollectablePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
}

// Called when the game starts or when spawned
void ACollectablePickup::BeginPlay()
{
	Super::BeginPlay();
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ACollectablePickup::OnOverlapBegin);
}

// Called every frame
void ACollectablePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectablePickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		Destroy();
	}
}

