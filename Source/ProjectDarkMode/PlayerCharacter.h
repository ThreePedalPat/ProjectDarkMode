// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStatComponent;

UCLASS()
class PROJECTDARKMODE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
    FVector LastCheckpoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Push system
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push")
    float PushRaycastDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Push")
    float PushRaycastRadius;

    UPROPERTY(BlueprintReadOnly, Category = "Push")
    class APushableActor* CurrentPushable;

    UPROPERTY(BlueprintReadOnly, Category = "Push")
    bool bIsPushing;

    UPROPERTY(BlueprintReadWrite, Category = "Moving")
    bool isMoving;


    void CheckForPushable();
    void StartPushing();
    void StopPushing();
    void Jump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    UStatComponent* Stats;

    // Movement
    void MoveForward(float Value);
    void MoveRight(float Value);
    void LookUp(float Value);
    void Turn(float Value);

    // Roll/Dodge
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float RollDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float RollDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float RollCooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsRolling;

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bCanRoll;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void PerformRoll();

private:
    FTimerHandle RollTimerHandle;
    FTimerHandle RollCooldownTimerHandle;

    void EndRoll();
    void ResetRollCooldown();

};
