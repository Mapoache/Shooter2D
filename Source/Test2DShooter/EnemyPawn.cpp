#include "EnemyPawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    // Create root capsule component
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CapsuleComponent;
    CapsuleComponent->SetCapsuleHalfHeight(96.0f);
    CapsuleComponent->SetCapsuleRadius(40.0f);
    CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
    // Create and attach the enemy mesh
    EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
    EnemyMesh->SetupAttachment(RootComponent);
    // Create and configure movement component
    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->MaxSpeed = MaxFlySpeed;
    MovementComponent->Acceleration = MaxAcceleration;
    MovementComponent->Deceleration = MaxAcceleration;
    MovementComponent->TurningBoost = 8.0f;
    // Create health component
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    // Initialize variables
    bCanFire = true;
    MovementSpeed = 100.0f;
    CirclingRadius = 1200.0f;
    CirclingSpeed = 2.0f;
}
// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
    Super::BeginPlay();
}
// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsInFireRange())
    {
        CircleAroundPlayer();
        Fire();
    }
    else
    {
        MoveTowardsPlayer();
    }
}

void AEnemyPawn::MoveTowardsPlayer()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector DirectionToPlayer = (PlayerLocation - CurrentLocation).GetSafeNormal();

        // Apply movement
        AddMovementInput(DirectionToPlayer, MovementSpeed);

        // Update rotation to face the player
        FRotator NewRotation = DirectionToPlayer.Rotation();
        SetActorRotation(NewRotation);
    }
}

void AEnemyPawn::CircleAroundPlayer()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        // Update angle
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        CurrentAngle += CirclingSpeed * DeltaTime;
        // Get player location
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        // Calculate position only along Y axis (side to side)
        FVector DesiredLocation = PlayerLocation +
            FVector(
                0.0f,  // Rotate along X axis
                FMath::Sin(CurrentAngle) * CirclingRadius,  // No Y movement
                0.0f   // No Z movement
            );
        // Calculate direction to desired position
        FVector DirectionToTarget = (DesiredLocation - GetActorLocation()).GetSafeNormal();
        // Apply movement
        AddMovementInput(DirectionToTarget, MovementSpeed);

        // Update rotation to face the player
        FRotator NewRotation = (PlayerLocation - GetActorLocation()).Rotation();
        SetActorRotation(NewRotation);
    }
}
void AEnemyPawn::Fire_Implementation()
{
    if (bCanFire)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn && ProjectileClass) // Check if ProjectileClass is set
        {
            // Calculate direction to player
            FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();

            // Spawn projectile
            FVector SpawnLocation = GetActorLocation() + Direction * 100.0f;
            FRotator SpawnRotation = Direction.Rotation();

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = this;
            AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
                ProjectileClass,
                SpawnLocation,
                SpawnRotation,
                SpawnParams
            );

            if (Projectile)
            {
                Projectile->Initialize(Direction, ProjectileSpeed);
            }
            // Set cooldown
            bCanFire = false;
            GetWorldTimerManager().SetTimer(
                FireCooldownTimerHandle,
                this,
                &AEnemyPawn::ResetFire,
                FireCooldownDuration,
                false
            );
        }
    }
}
void AEnemyPawn::ResetFire()
{
    bCanFire = true;
}
bool AEnemyPawn::IsInFireRange()
{
    // Implement fire range check logic here
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
        return Distance <= 600.0f; // Adjust the range as needed
    }
    return false;
}