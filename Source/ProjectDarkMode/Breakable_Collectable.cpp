// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable_Collectable.h"
#include "PlayerCharacter.h"

// Sets default values
ABreakable_Collectable::ABreakable_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	rotator = CreateDefaultSubobject<URotatorComponent>(TEXT("Rotator"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void ABreakable_Collectable::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Collectable");
	rotator->rot = rot;
	health = 2;
	
}

// Called every frame
void ABreakable_Collectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable_Collectable::GetDamaged(APlayerCharacter* character, int32 damage)
{
	health -= damage;
	//change mesh to damaged based on health
	if (health == 1)
	{
		mesh->SetMaterial(0, damagedMat);
	}

	if (health < 1)
	{
		Collect(character);
	}

	//print health
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, "Health: " + FString::FromInt(health));
}

void ABreakable_Collectable::Collect(APlayerCharacter* character)
{
	character->CollectableRecieved();
	Destroy();
}


