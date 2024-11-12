#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "HealthComponent.h" // Include the HealthComponent header
#include "EnemyPawn.generated.h"

UCLASS()
class TEST2DSHOOTER_API AEnemyPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AEnemyPawn();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Movement functions
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveTowardsPlayer();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void CircleAroundPlayer();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MovementSpeed = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MaxAcceleration = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MaxFlySpeed = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float CirclingRadius = 500.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float CirclingSpeed = 2.0f;

    // Expose properties to Blueprint
    UFUNCTION(BlueprintNativeEvent, Category = "Firing")
    void Fire();
    virtual void Fire_Implementation(); // Define Fire implementation with the correct suffix

    // Expose projectile properties to blueprint
    UPROPERTY(BlueprintReadWrite, Category = "Firing", meta = (ExposeOnSpawn = true))
    TSubclassOf<class AProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    float ProjectileSpeed = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    float FireCooldownDuration = 1.0f; // Initialize to a default value

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firing")
    bool bCanFire = true; // Initialize to true to allow firing initially

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    UHealthComponent* HealthComponent;

private:
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* EnemyMesh;

    UPROPERTY(VisibleAnywhere)
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(VisibleAnywhere)
    FTimerHandle FireCooldownTimerHandle;

    void ResetFire();
    bool IsInFireRange();

    float CurrentAngle = 0.0f;
};
