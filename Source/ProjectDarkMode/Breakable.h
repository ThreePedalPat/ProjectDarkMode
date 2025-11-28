// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Breakable.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDARKMODE_API UBreakable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBreakable();

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* BreakableParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Cam Shake")
	TSubclassOf<UCameraShakeBase> breakableCamShake;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
