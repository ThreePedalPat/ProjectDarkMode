// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTDARKMODE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

    // Base stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float JumpPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int Defense;

    // Stat modifiers (from items/talents)
    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float MoveSpeedModifier;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float JumpPowerModifier;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int AttackDamageModifier;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int DefenseModifier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    bool onFire;

    float fireTimer;
    float fireTimeRemaining;

    // Functions to get final stats (base + modifiers)
    UFUNCTION(BlueprintCallable, Category = "Stats")
    float GetMoveSpeed() const { return MoveSpeed * (1.0f + MoveSpeedModifier); }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    float GetJumpPower() const { return JumpPower * (1.0f + JumpPowerModifier); }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    int GetAttackDamage() const { return AttackDamage * (1.0f + AttackDamageModifier); }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    int GetDefense() const { return Defense * (1.0f + DefenseModifier); }

    UFUNCTION(BlueprintCallable, Category = "Status")
    void SetOnFire();

    // Apply stat modifier (from items/talents)
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddStatModifier(FName StatName, float Value);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void RemoveStatModifier(FName StatName, float Value);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void TakeDamage(int damage);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void Death();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    APlayerCharacter* myBody;
    FTimerHandle hurtResetTimer;

    void ResetHurt();
		
};
