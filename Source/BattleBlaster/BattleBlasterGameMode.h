// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATank* Tank;
	int32 TowerCount; // when this reaches to zero we'll stop the game and display victory

	bool IsGameOver = false;
	bool IsVictory = false;

	void ActorDied(AActor* DeadActor); // this will be called when tank or towers health become zero and we need to certain things to it
	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.0f;

	void OnGameOverTimerTimeout();

	
};
