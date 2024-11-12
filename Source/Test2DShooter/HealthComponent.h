#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnDamaged, float, DamageAmount);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST2DSHOOTER_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    // Sets default values for this component's properties
    UHealthComponent();
protected:
    // Called when the game starts
    virtual void BeginPlay() override;
public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    // Expose health properties to Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float CurrentHealth;
    // Function to handle damage
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount);
    // Function to handle health regeneration
    UFUNCTION(BlueprintCallable, Category = "Health")
    void RegenerateHealth(float DeltaTime);
    // Event when the pawn receives damage
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnPawnDamaged OnPawnDamaged;
    // Event when the pawn dies
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnPawnDied OnPawnDied;
private:
    float LastDamageTime;
    float HealthRegenRate;
};