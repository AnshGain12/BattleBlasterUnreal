// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "Tank.h"

#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATower : public ABasePawn
{
	GENERATED_BODY() 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	ATower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	ATank* Tank; // the tower is going to use this pointer in order to rotate towards the tank

	UPROPERTY(EditAnywhere)
	float FiringRange = 500.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	void CheckFireCondition();
	bool IsInFireRange();
	void HandleDestruction();

};
