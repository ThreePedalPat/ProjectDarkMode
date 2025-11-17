// Fill out your copyright notice in the Description page of Project Settings.


#include "DJ.h"
#include "Components/AudioComponent.h"

// Sets default values
ADJ::ADJ()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	source = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	source->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADJ::BeginPlay()
{
	Super::BeginPlay();
	source->Sound = mainSong;
	source->Play();
}

// Called every frame
void ADJ::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

