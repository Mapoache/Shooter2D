#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "SpaceShipPawn.generated.h"

UCLASS()
class TEST2DSHOOTER_API ASpaceShipPawn : public APawn
{
    GENERATED_BODY()
public:
    // Sets default values for this pawn's properties
    ASpaceShipPawn();
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    // Movement functions
    void MoveUp(float Value);
    void MoveRight(float Value);
    void Fire();
    void ResetFire();

    // Event handlers
    UFUNCTION(BlueprintImplementableEvent, Category = "Firing")
    void OnFirePressed(FVector AimDirection);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent* RootSceneComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UCapsuleComponent* CapsuleComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ShipMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UFloatingPawnMovement* MovementComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float MovementSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
    float FireCooldownDuration;

private:

    UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float IdleUpwardSpeed = 20.0f;  // Adjustable in editor
    bool IsMoving() const;  // Helper function to check if there's any movement input
    bool bCanFire;
    FTimerHandle FireCooldownTimerHandle;
};