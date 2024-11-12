#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TEST2DSHOOTER_API AProjectile : public AActor
{
    GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AProjectile();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Function to initialize the projectile
    void Initialize(const FVector& Direction, float Speed);
private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* ProjectileMesh;
    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};