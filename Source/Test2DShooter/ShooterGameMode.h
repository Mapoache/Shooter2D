#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

UCLASS()
class TEST2DSHOOTER_API AShooterGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    // Sets default values for this game mode's properties
    AShooterGameMode();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    // Function to handle player death
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void HandlePlayerDeath();
    // Function to handle player win
    UFUNCTION(BlueprintCallable, Category = "GameMode")
    void HandlePlayerWin();
    // Expose properties to Blueprint
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
    int32 WinHeight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
    int32 Wins;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
    int32 Losses;
private:
    void RestartGame();
};