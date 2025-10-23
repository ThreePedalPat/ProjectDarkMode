// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APushableActor::APushableActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create mesh component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void APushableActor::BeginPlay()
{
    Super::BeginPlay();
    GroundZ = GetActorLocation().Z; // Adjust this to your ground level
}

void APushableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Prevent box from going underground
    FVector CurrentLocation = GetActorLocation();
}

void APushableActor::ApplyPush(FVector Direction, float DeltaTime)
{

}

void APushableActor::StopPushing()
{
    //MeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, 0));
}

void APushableActor::SnapToGridPosition()
{

}