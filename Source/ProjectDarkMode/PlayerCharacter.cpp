// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Camera/CameraTypes.h"
#include "PushableActor.h"
#include "Breakable.h"

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

    JumpMaxHoldTime = 0.3f;

    // Don't rotate character with camera
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

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
    if (isStunned)
    {
        //temporary fix to infinite stun bug
        //stunTimer += DeltaTime;
        //if (stunTimer > 1.5f)
        //{
        //    isStunned = false;
        //    stunTimer = 0;
        //}
        GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, UKismetStringLibrary::Conv_BoolToString(isStunned) + UKismetStringLibrary::Conv_BoolToString(triggerCrash) + UKismetStringLibrary::Conv_BoolToString(isCharging));
         
    }
    // Update movement speed from stats
    if (Stats)
    {
        GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed();
        GetCharacterMovement()->JumpZVelocity = Stats->GetJumpPower();
    }

    if (isCharging)
    {
        AddMovementInput(GetActorForwardVector(), 100);

        // Raycast in front of character to detect obstacles
        FVector Start = GetActorLocation();
        FVector ForwardVector = GetActorForwardVector();
        FVector End = Start + (ForwardVector * 60);
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
            FCollisionShape::MakeSphere(50),
            QueryParams
        );

        if (bHit)
        {
            if (HitResult.GetActor())
            {
                AActor* hitActor = HitResult.GetActor();
                APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
                if (hitActor->ActorHasTag("Enemy"))
                {
                    //continue charging and launch enemy/deal damage
                }
                else if (hitActor->ActorHasTag("Breakable"))
                {
                    UBreakable* actorBreakable = hitActor->GetComponentByClass<UBreakable>();
                    if (actorBreakable)
                    {
                        if (actorBreakable->BreakableParticleSystem)
                        {
                            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                                GetWorld(),
                                actorBreakable->BreakableParticleSystem,
                                hitActor->GetActorLocation(),
                                FRotator::ZeroRotator,
                                FVector(1.0f),
                                true
                            );
                        }

                        if (actorBreakable->breakableCamShake)
                        {
                            //shake cam according to breakable
                            camManager->StartCameraShake(actorBreakable->breakableCamShake, 1.0f);
                        }
                    }

                    hitActor->Destroy();
                }
                else
                {

                    //play crash cam shake
                    camManager->StartCameraShake(crashCamShake, 1.0f);

                    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "You hit a wall!");
                    triggerCrash = true;
                    isStunned = true;
                    StopCharge();
                    // Reset after a short delay (0.1 seconds)
                        GetWorld()->GetTimerManager().SetTimer(
                            CrashResetTimer,
                            this,
                            &APlayerCharacter::ResetCrashTrigger,
                            0.1f,
                            false
                        );
                }
            }
        }
    }

    isMoving = GetCharacterMovement()->IsMovingOnGround();
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

    // Charge
    PlayerInputComponent->BindAction("Charge", IE_Pressed, this, &APlayerCharacter::StartCharge);
    PlayerInputComponent->BindAction("Charge", IE_Released, this, &APlayerCharacter::StopCharge);

    // Stomp
    PlayerInputComponent->BindAction("Stomp", IE_Pressed, this, &APlayerCharacter::Stomp);
}

void APlayerCharacter::Jump()
{
    ACharacter::Jump();
    //bIsPushing = false;
    //CurrentPushable = NULL;
}



void APlayerCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.0f && !isCharging && !isStunned)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.0f && !isCharging && !isStunned)
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
    if (isCharging)
    {
        AddControllerYawInput(Value * chargeTurnMod);
    }
    else
    {
        AddControllerYawInput(Value);
    }
}

void APlayerCharacter::StartCharge()
{
    if (!isStunned)
    {
        isCharging = true;
        normalSpeed = GetCharacterMovement()->MaxWalkSpeed;
        GetCharacterMovement()->MaxWalkSpeed = chargeSpeed;
        bUseControllerRotationYaw = true;
    }
}

void APlayerCharacter::StopCharge()
{
    if (isCharging)
    {
        isCharging = false;
        GetCharacterMovement()->MaxWalkSpeed = normalSpeed;
        bUseControllerRotationYaw = false;
    }
}


void APlayerCharacter::ResetCrashTrigger()
{
    triggerCrash = false;
}

void APlayerCharacter::ResetHurtTrigger()
{
    triggerHurt = false;
}

void APlayerCharacter::ResetStompTrigger()
{
    triggerStomp = false;
}

void APlayerCharacter::Stomp()
{
    triggerStomp = true;


    GetWorld()->GetTimerManager().SetTimer(stompResetTimer, this, &APlayerCharacter::ResetStompTrigger, 0.1f, false);
}

void APlayerCharacter::StompDamage()
{
    FCollisionShape sphere = FCollisionShape::MakeSphere(stompRange);
    FCollisionQueryParams query;
    query.AddIgnoredActor(this);
    TArray<FHitResult> hit;

    if (StompParticleSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            StompParticleSystem,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.0f),
            true
        );
    }

    APlayerCameraManager* camMan = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
    camMan->StartCameraShake(stompCamShake, 1.0f);


    bool bHit = GetWorld()->SweepMultiByChannel(hit, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Visibility, sphere, query);

    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Stomped and shaken!");

    if (bHit)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Stomp hit something!");
        for (size_t i = 0; i < hit.Num(); i++)
        {
            if (hit[i].GetActor())
            {
                AActor* hitActor = hit[i].GetActor();
                if (hitActor->ActorHasTag("Enemy"))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Destroyed an enemy!");
                    hitActor->Destroy();
                }
            }
        }
    }
}


