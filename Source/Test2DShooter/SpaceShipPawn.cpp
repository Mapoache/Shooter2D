#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "SpaceShipPawn.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpaceShipPawn::ASpaceShipPawn()
{
    // Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // Create the capsule component and set it as the root component
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;
    CapsuleComponent->SetCapsuleHalfHeight(96.0f);
    CapsuleComponent->SetCapsuleRadius(40.0f);
    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
    // Create and attach the static mesh to the capsule
    ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
    ShipMesh->SetupAttachment(CapsuleComponent);
    ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Only the capsule handles collision
    // Create and attach the movement component
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = CapsuleComponent; // Ensure movement respects collisions
    // Create and attach the spring arm component
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(CapsuleComponent);
    SpringArmComponent->TargetArmLength = 500.0f; // Adjust this value as needed
    SpringArmComponent->bUsePawnControlRotation = false; // Fix the camera rotation
    // Create and attach the camera component
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    CameraComponent->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
    CameraComponent->SetProjectionMode(ECameraProjectionMode::Perspective);
    // Create and attach the health component
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // Initialize firing variables
    bCanFire = true;
    // Default values for editable properties
    MovementSpeed = 100.0f;
    FireCooldownDuration = 0.5f;
}
// Called when the game starts or when spawned
void ASpaceShipPawn::BeginPlay()
{
    Super::BeginPlay();
}
// Called every frame
void ASpaceShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (PC)
    {
        // Your existing rotation code here...
        // Add idle upward movement when there's no input
        if (!IsMoving())
        {
            // Add upward movement
            AddMovementInput(FVector(0.0f, 0.0f, 1.0f), IdleUpwardSpeed * DeltaTime);
        }
    }
}

// Called to bind functionality to input
void ASpaceShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveUp", this, &ASpaceShipPawn::MoveUp);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceShipPawn::MoveRight);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASpaceShipPawn::Fire);
}
void ASpaceShipPawn::MoveUp(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value * MovementSpeed);
    }
}
void ASpaceShipPawn::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value * MovementSpeed);
    }
}
void ASpaceShipPawn::Fire()
{
    if (bCanFire)
    {
        // Set cooldown
        bCanFire = false;
        GetWorldTimerManager().SetTimer(FireCooldownTimerHandle, this, &ASpaceShipPawn::ResetFire, FireCooldownDuration, false);
        FVector2D MousePosition;
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController)
        PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
        FVector WorldPosition, WorldDirection;
        PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldPosition, WorldDirection);
        FVector AimDirection = WorldDirection * 10000.0f; // Adjust the multiplier as needed
        OnFirePressed(AimDirection);
    }
}
void ASpaceShipPawn::ResetFire()
{
    bCanFire = true;
}
bool ASpaceShipPawn::IsMoving() const
{
    // Check if there's any movement input
    return !GetPendingMovementInputVector().IsNearlyZero();
}