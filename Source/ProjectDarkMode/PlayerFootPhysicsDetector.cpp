// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFootPhysicsDetector.h"
#include "PushableActor.h"

// Sets default values
APlayerFootPhysicsDetector::APlayerFootPhysicsDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the box component
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	// Set the box size
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
}

// Called when the game starts or when spawned
void APlayerFootPhysicsDetector::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlayerFootPhysicsDetector::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlayerFootPhysicsDetector::OnOverlapEnd);
	
}

void APlayerFootPhysicsDetector::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APushableActor* mesh = Cast<APushableActor>(OtherActor);
	if (mesh)
	{
		mesh->MeshComponent->SetSimulatePhysics(false);
	}
}

void APlayerFootPhysicsDetector::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APushableActor* mesh = Cast<APushableActor>(OtherActor);
	if (mesh)
	{
		mesh->MeshComponent->SetSimulatePhysics(true);
	}
}

// Called every frame
void APlayerFootPhysicsDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

