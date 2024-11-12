
#include "HealthComponent.h"
#include "Engine/World.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // Default values for health properties
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    HealthRegenRate = 4.0f; // Health regenerates at 4 units per second
}
// Called when the game starts
void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize last damage time
    LastDamageTime = GetWorld()->GetTimeSeconds();
}
// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Regenerate health if no damage has been taken for 2 seconds
    if (GetWorld()->GetTimeSeconds() - LastDamageTime > 2.0f)
    {
        RegenerateHealth(DeltaTime);
    }
}
void UHealthComponent::TakeDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;
    LastDamageTime = GetWorld()->GetTimeSeconds();
    // Broadcast the OnPawnDamaged event
    OnPawnDamaged.Broadcast(DamageAmount);
    if (CurrentHealth <= 0.0f)
    {
        // Handle death
        CurrentHealth = 0.0f;
        // Broadcast the OnPawnDied event
        OnPawnDied.Broadcast();
        // Implement death logic here
    }
}
void UHealthComponent::RegenerateHealth(float DeltaTime)
{
    CurrentHealth += HealthRegenRate * DeltaTime;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
}