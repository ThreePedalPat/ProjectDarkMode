// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "RotatorComponent.h"
#include "Breakable_Collectable.generated.h"

UCLASS()
class PROJECTDARKMODE_API ABreakable_Collectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakable_Collectable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* mesh;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	UMaterial* damagedMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotator")
	FRotator rot;

	int health;

	void GetDamaged(APlayerCharacter* character, int32 damage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Collect(APlayerCharacter* character);

	URotatorComponent* rotator;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
