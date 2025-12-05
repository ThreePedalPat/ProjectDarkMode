// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable_Collectable.h"

// Sets default values
ABreakable_Collectable::ABreakable_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void ABreakable_Collectable::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Collectable");
	
}

// Called every frame
void ABreakable_Collectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable_Collectable::GetDamaged()
{
	health--;
	//change mesh to damaged based on health


	//print health
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, "Health: " + FString::FromInt(health));
}

void ABreakable_Collectable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (health < 1)
	{
		Destroy();
	}
}

