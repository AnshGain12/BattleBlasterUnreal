// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	// setting a timer to shoot the projectile via tower
	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true); // here the true at end means looping is enabled
}

ATower::ATower()
{
}

void ATower::Tick(float DeltaTime)
{
	if (IsInFireRange()) {
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire condition is activating in every 2sec"));

	// step 1 -> check whether the tank is valid or not 
	if (Tank) {
		// step2 -> check whether the tank is close enough to fire or not
		if (IsInFireRange() && Tank->IsAlive) {
			Fire();
		}
	}
}

bool ATower::IsInFireRange()
{
	FVector TankLoc = Tank->GetActorLocation();
	FVector TowerLoc = GetActorLocation();
	float TankTowerDistance = FVector::Dist(TankLoc, TowerLoc);
	if (Tank && (TankTowerDistance <= FiringRange)) {
		return true;
	}
	else {
		return false;
	}
}


void ATower::HandleDestruction() {
	Super::HandleDestruction();
	UE_LOG(LogTemp, Warning, TEXT("TOWER HANDLE DESTRUCTION"));
}
	
