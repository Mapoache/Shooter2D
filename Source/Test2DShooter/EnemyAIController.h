#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class TEST2DSHOOTER_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float FireRange = 2000.0f; // Increased from 1000 to 2000
protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY()
    class AEnemyPawn* ControlledPawn;
    UPROPERTY(EditAnywhere, Category = "AI")
    float CirclingRadius = 1200.0f;
    bool IsInFireRange() const;
    void UpdateBehavior();

};