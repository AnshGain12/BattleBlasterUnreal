// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 index)
{
	if (index > 0 && index <= LastLevelIndex) {
		CurrentLevelIndex = index;

		FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelIndex); // name of the next level , here name of the level must match with the bleprint name
		
		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);


	}
}

void UBattleBlasterGameInstance::LoadNextLevel() {
	if (CurrentLevelIndex < LastLevelIndex) {
		ChangeLevel(CurrentLevelIndex + 1);
	}
	else {
		RestartGame();
	}
}


void UBattleBlasterGameInstance::RestartCurrentLevel() {
	//UGameplayStatics::OpenLevel(GetWorld(), FName("%s"), CurrentLevelIndex);
	ChangeLevel(CurrentLevelIndex);
}


void UBattleBlasterGameInstance::RestartGame() {
	//UGameplayStatics::OpenLevel(GetWorld(), FName("%s"), CurrentLevelIndex)
	ChangeLevel(1);
}
