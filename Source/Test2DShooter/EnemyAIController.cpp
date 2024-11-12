#include "EnemyAIController.h"
#include "EnemyPawn.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}
void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ControlledPawn = Cast<AEnemyPawn>(InPawn);
}
void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!ControlledPawn)
    {
        ControlledPawn = Cast<AEnemyPawn>(GetPawn());
        return;
    }
    UpdateBehavior();
}
void AEnemyAIController::UpdateBehavior()
{
    if (!ControlledPawn)
        return;
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
        return;
    // Debug lines
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
       if (IsInFireRange())
    {
        ControlledPawn->CircleAroundPlayer();
        ControlledPawn->Fire();
    }
    else
    {
       ControlledPawn->MoveTowardsPlayer();
    }
}
bool AEnemyAIController::IsInFireRange() const
{
    if (!ControlledPawn)
        return false;
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
        return false;
    float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
    return Distance <= FireRange;
}
