// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "TimerManager.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Warning, TEXT("no of towers: %d"), TowerCount);
	// for looking for tank we can also use the same function as above but since we know that there is only one tank , so we can use some other function which is more optimized for this purpose
	// we will use the function called "GetPlayerPawn()" for this purpose
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // it takes two arg , the current game world and index of the pawn , by default since we have only one player pawn so it will be zero 
	// casting player pawn to tank object
	if (PlayerPawn) {
		Tank = Cast<ATank>(PlayerPawn);
		if (Tank) {
			// now we will loop through the array and set all the towers to move toward the tank
			
			int32 LoopIndex = 0;
			while (LoopIndex < TowerCount) {
				// pointing all the towers toward the tank
				
				if (AActor* TowerActor = Towers[LoopIndex]) {
					if (ATower* Tower = Cast<ATower>(TowerActor)) {
						Tower->Tank = Tank; // setting tank variable of this class as value for tank of tower class
					}
				}
				LoopIndex++;
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("casting of pawn to player is unsuccessful!"));
		}
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	bool IsVictory = false;
	// check the type of the actor that died whether it is tank , tower
	if (DeadActor == Tank) {
		// tank died
		IsGameOver = true;
		Tank->HandleDestruction();
		//UE_LOG(LogTemp, Warning, TEXT("tank died defeat!"));
	}
	else {
		// tower died
		UE_LOG(LogTemp, Warning, TEXT("tower died "));
		// for tower we need to cast the deadActor into tower , we dont need to do it for tank bcz a pointer to it is already there in the class
		
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower) {
			DeadTower->HandleDestruction();
			DeadTower->Destroy();
			TowerCount--;
			if (!TowerCount) {
				IsGameOver = true;
				IsVictory = true;
				//UE_LOG(LogTemp, Warning, TEXT("Victory"));
			
			}
		}

	}
	if (IsGameOver) {
		FString GameOverString = IsVictory ? "Victory" : "Defeat";
		UE_LOG(LogTemp, Warning, TEXT("Game Over: %s"), *GameOverString);
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(
			GameOverTimerHandle,
			this,
			&ABattleBlasterGameMode::OnGameOverTimerTimeout, // call this function when timer gets over
			GameOverDelay,
			false
		);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	//UE_LOG(LogTemp, Warning, TEXT("Game over timer timerout"));

	// restart the game  -> we will use a function fromm ugameplaystatics class 
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld()); // getting the current level name to pass it to below function so that it can restart the level that we want 
	UGameplayStatics::OpenLevel(GetWorld(), *CurrentLevelName);

}
