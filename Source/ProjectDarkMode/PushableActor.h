// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PushableActor.generated.h"

UCLASS()
class PROJECTDARKMODE_API APushableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // Push settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Settings")
    float PushForce;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Settings")
    bool bCanBePushed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Settings")
    bool bSnapToGrid; // For puzzle alignment

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push Settings", meta = (EditCondition = "bSnapToGrid"))
    float GridSize;

    // PushableActor.h - add this property:
    UPROPERTY(BlueprintReadOnly, Category = "Push State")
    bool bPhysicsEnabled;

    float GroundZ;

    bool isBeingPushed;

    // Called by player to apply push
    UFUNCTION(BlueprintCallable, Category = "Push")
    void ApplyPush(FVector Direction, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Push")
    void StopPushing();

private:
    FVector CurrentVelocity;
    float SmoothingSpeed;
    FVector TargetGridPosition;
    bool bMovingToGrid;
    float GridMoveSpeed;

    void SnapToGridPosition();

};
