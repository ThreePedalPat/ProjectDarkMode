// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatComponent.h"
#include "PushableActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create spring arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.0f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 10.0f;

    // Create camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    // Create stats component
    Stats = CreateDefaultSubobject<UStatComponent>(TEXT("Stats"));

    // Character movement setup
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.6f;
    GetCharacterMovement()->GravityScale = 1.5f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

    // Don't rotate character with camera
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Roll setup
    RollDistance = 250.0f;
    RollDuration = 0.4f;
    RollCooldown = 1.0f;
    bIsRolling = false;
    bCanRoll = true;

    //Push system
    PushRaycastDistance = 50.0f;
    PushRaycastRadius = 30.0f;
    CurrentPushable = nullptr;
    bIsPushing = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    LastCheckpoint = GetActorLocation();
    // Apply stats to character movement
    if (Stats)
    {
        GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed();
        GetCharacterMovement()->JumpZVelocity = Stats->GetJumpPower();
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Update movement speed from stats
    if (Stats && !bIsRolling)
    {
        float SpeedMultiplier = bIsPushing ? 0.3f : 1.0f; // 30% speed when pushing
        GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed() * SpeedMultiplier;
        GetCharacterMovement()->JumpZVelocity = Stats->GetJumpPower();
    }

    //// Check for pushables in front of us
    CheckForPushable();
    isMoving = GetCharacterMovement()->IsMovingOnGround();
    //// If we're pushing and moving forward, apply push force
    //if (bIsPushing && CurrentPushable )
    //{
    //    UPrimitiveComponent* FloorComponent = GetCharacterMovement()->GetMovementBase();

    //    // Only trigger push if character is NOT standing on this box
    //    if (FloorComponent != CurrentPushable->MeshComponent)
    //    {
    //        FVector InputDirection = GetLastMovementInputVector();
    //        if (InputDirection.SizeSquared() > 0.01f && !GetCharacterMovement()->IsFalling())
    //        {
    //            CurrentPushable->ApplyPush(GetActorForwardVector(), DeltaTime);
    //           /* CurrentPushable->isBeingPushed = true;*/
    //        }
    //    }
    //    //else
    //    //{
    //    //    CurrentPushable->StopPushing();
    //    //    CurrentPushable->isBeingPushed = false;
    //    //}
    //}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // Camera
    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

    // Jump
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Roll
    PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APlayerCharacter::PerformRoll);

}

void APlayerCharacter::Jump()
{
    ACharacter::Jump();
    //bIsPushing = false;
    //CurrentPushable = NULL;
}



void APlayerCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f && !bIsRolling)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f && !bIsRolling)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void APlayerCharacter::PerformRoll()
{
    if (!bCanRoll || bIsRolling || bIsPushing)
        return;

    bIsRolling = true;
    bCanRoll = false;

    // Get roll direction (current movement direction or forward if standing still)
    FVector RollDirection = GetVelocity();
    if (RollDirection.SizeSquared() < 1.0f)
    {
        RollDirection = GetActorForwardVector();
    }
    RollDirection.Z = 0.0f;
    RollDirection.Normalize();

    // Disable friction during roll
    GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
    GetCharacterMovement()->GroundFriction = 0.0f;

    // Launch character in roll direction
    LaunchCharacter(RollDirection * (RollDistance / RollDuration), false, false);

    // Set timer to end roll
    GetWorldTimerManager().SetTimer(RollTimerHandle, this, &APlayerCharacter::EndRoll, RollDuration, false);
}

void APlayerCharacter::EndRoll()
{
    bIsRolling = false;

    // Restore friction
    GetCharacterMovement()->BrakingFrictionFactor = 2.0f; // Default value
    GetCharacterMovement()->GroundFriction = 8.0f; // Default value

    // Start cooldown
    GetWorldTimerManager().SetTimer(RollCooldownTimerHandle, this, &APlayerCharacter::ResetRollCooldown, RollCooldown, false);
}

void APlayerCharacter::ResetRollCooldown()
{
    bCanRoll = true;
}

void APlayerCharacter::CheckForPushable()
{
    if (GetCharacterMovement()->IsFalling())
    {
        if (CurrentPushable)
        {
            StopPushing();
        }
        return;
    }

    // Raycast in front of character
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * PushRaycastDistance);

    // Use a sphere trace for more forgiving detection
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity,
        ECC_WorldDynamic,
        FCollisionShape::MakeSphere(PushRaycastRadius),
        QueryParams
    );

    if (bHit)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Pushable Found!");
        APushableActor* HitPushable = Cast<APushableActor>(HitResult.GetActor());

        if (HitPushable && HitPushable != CurrentPushable)
        {
            StartPushing();
            CurrentPushable = HitPushable;
        }
        else if (!HitPushable && CurrentPushable)
        {
            StopPushing();
        }
    }
    else if (CurrentPushable)
    {
        StopPushing();
    }
}

void APlayerCharacter::StartPushing()
{
    bIsPushing = true;
}

void APlayerCharacter::StopPushing()
{
    bIsPushing = false;

    if (CurrentPushable)
    {
        CurrentPushable->StopPushing();
        CurrentPushable = nullptr;
    }
}

