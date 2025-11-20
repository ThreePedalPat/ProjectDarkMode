// Fill out your copyright notice in the Description page of Project Settings.

#include "StatComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    // Default values
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MoveSpeed = 600.0f;
    JumpPower = 600.0f;
    AttackDamage = 10;
    Defense = 5;

    // Initialize modifiers
    MoveSpeedModifier = 0.0f;
    JumpPowerModifier = 0.0f;
    AttackDamageModifier = 0.0f;
    DefenseModifier = 0.0f;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

    CurrentHealth = MaxHealth;

	// ...
	
}

void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Ticking!");
    if (onFire)
    {
        fireTimer += DeltaTime;
        fireTimeRemaining -= DeltaTime;
        if (fireTimer >= 0.2f)
        {
            TakeDamage(2);
            fireTimer = 0;
        }
        if (fireTimeRemaining <= 0)
        {
            onFire = false;
        }
    }
}

void UStatComponent::SetOnFire()
{
    onFire = true;
    fireTimer = 0;
    fireTimeRemaining = 2.0f;
}

void UStatComponent::AddStatModifier(FName StatName, float Value)
{
    if (StatName == "MoveSpeed")
        MoveSpeedModifier += Value;
    else if (StatName == "JumpPower")
        JumpPowerModifier += Value;
    else if (StatName == "AttackDamage")
        AttackDamageModifier += Value;
    else if (StatName == "Defense")
        DefenseModifier += Value;
}

void UStatComponent::RemoveStatModifier(FName StatName, float Value)
{
    AddStatModifier(StatName, -Value);
}

void UStatComponent::TakeDamage(int damage)
{
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Taking Damage!");
    damage -= GetDefense();
    if (CurrentHealth - damage < 1)
    {
        Death();
    }
    else
    {
        CurrentHealth -= damage;
        if (myBody)
        {
            myBody->triggerHurt = true;
            GetWorld()->GetTimerManager().SetTimer(hurtResetTimer, this, &UStatComponent::ResetHurt, 0.1f, false);
        }
        else
        {
            myBody = Cast<APlayerCharacter>(GetOwner());
            myBody->triggerHurt = true;
            GetWorld()->GetTimerManager().SetTimer(hurtResetTimer, this, &UStatComponent::ResetHurt, 0.1f, false);
        }
    }
}

void UStatComponent::Death()
{
    CurrentHealth = 0;
    GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, "You Died...Noob");
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UStatComponent::ResetHurt()
{
    myBody->ResetHurtTrigger();
}


