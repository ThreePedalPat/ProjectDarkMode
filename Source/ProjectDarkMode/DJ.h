// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DJ.generated.h"

UCLASS()
class PROJECTDARKMODE_API ADJ : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADJ();

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundWave* mainSong;

	UPROPERTY(EditAnywhere, Category = "Audio")
	UAudioComponent* source;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
