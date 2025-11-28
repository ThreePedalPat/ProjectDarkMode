// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable.h"

// Sets default values for this component's properties
UBreakable::UBreakable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBreakable::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->Tags.Num() > 0)
	{
		GetOwner()->Tags.Empty();
	}
	if (!GetOwner()->ActorHasTag("Breakable"))
	{
		GetOwner()->Tags.Add("Breakable");
	}
	// ...
	
}


// Called every frame
void UBreakable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

