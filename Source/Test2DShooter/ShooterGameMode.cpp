#include "ShooterGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShooterGameMode::AShooterGameMode()
{
    // Set this game mode to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // Default values for editable properties
    WinHeight = 10000;
    Wins = 0;
    Losses = 0;
}
// Called when the game starts or when spawned
void AShooterGameMode::BeginPlay()
{
    Super::BeginPlay();
}
// Called every frame
void AShooterGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Check if the player has reached the win height
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        if (PlayerPawn->GetActorLocation().Z >= WinHeight)
        {
            HandlePlayerWin();
        }
    }
}
void AShooterGameMode::HandlePlayerDeath()
{
    Losses++;
    RestartGame();
}
void AShooterGameMode::HandlePlayerWin()
{
    Wins++;
    RestartGame();
}
void AShooterGameMode::RestartGame()
{
    // Restart the game
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}