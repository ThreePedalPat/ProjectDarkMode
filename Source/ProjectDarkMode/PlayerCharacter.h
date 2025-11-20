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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
    FVector LastCheckpoint;

    UPROPERTY(BlueprintReadWrite)
    bool isStunned;

    UPROPERTY(BlueprintReadWrite)
    bool isCharging;

    UPROPERTY(BlueprintReadWrite)
    bool triggerCrash;

    UPROPERTY(BlueprintReadWrite)
    bool triggerHurt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Ability")
    float chargeSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Ability")
    float chargeTurnMod;

    void ResetHurtTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, Category = "Moving")
    bool isMoving;

    void StartCharge();
    void StopCharge();
    void ResetCrashTrigger();

    FTimerHandle CrashResetTimer;
    void Jump();

private:
    float normalSpeed;
};
