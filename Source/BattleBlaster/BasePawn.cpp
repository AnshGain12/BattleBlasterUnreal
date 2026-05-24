// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh); // we are adding it to turret mesh bcz when we are rotating the turret mesh , we need to rotate the projectile point as well 

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	// now we will get direction of this vector
	
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f); // return the rotation of this vector as an FRotator object
	// to make rotation smoother
	// ToTarget.Rotation()-> it converts the vector into Euler angles.
	FRotator InterpolatedRotation = FMath::RInterpTo(
		TurretMesh->GetComponentRotation(), 
		LookAtRotation, 
		GetWorld()->GetDeltaSeconds(),
		10.0f);
	TurretMesh->SetWorldRotation(InterpolatedRotation);
	//TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
	//DrawDebugSphere(GetWorld(), SpawnLocation, 20.0f,20, FColor::Blue, false , 3.0f);
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	// now we'll set the owner of the projectile
	if (Projectile) {
		Projectile->SetOwner(this); // let say tank fire the projectile -> then we know that the tank is a basepawn and it is going to call the fire function , so it will set the owner of the projectile to bp_tank as it is calling the fire function
		// this keyword refers to the class that calls the fire function 


		/* AActor* ProjectileOwner = Projectile->GetOwner(); // return the owner of the projectile
		if (ProjectileOwner) {
		UE_LOG(LogTemp, Display, TEXT("Projectile owner: %s"), *ProjectileOwner->GetActorNameOrLabel());
		}*/
	}
}

void ABasePawn::HandleDestruction()
{
	UE_LOG(LogTemp, Warning, TEXT("BASEPAWN HANDLE DESTRUCTION"));
}
