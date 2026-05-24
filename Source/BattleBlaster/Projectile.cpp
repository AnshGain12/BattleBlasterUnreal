// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletProjectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileComponent"));
	SetRootComponent(BulletProjectile);

    // Use this component to drive projectile's movement.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->InitialSpeed = 1000.0f;
    ProjectileMovementComponent->MaxSpeed = 1000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->Bounciness = 0.0f;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    // binding the OnHit function -> when projectile mesh hit or block another function this OnHit function will be called 
    BulletProjectile->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); // hold ctrl key and enter on it to look for all the arguments that it require
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// bind this function to OnComponenHit delegate
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
    FVector NormalImpulse, const FHitResult& Hit)
{
    //// HitComponent -> static mesh component , OtherActor -> the actor that it is hitting , OtherComp -> the compoent in other actor
    //// that we hit , 
    //if (OtherActor) {
    //    // in BP_Projectile , enable Simulation Generation Hit Events, also change object type to be world dynamic in collision setting 
    //    UE_LOG(LogTemp, Warning, TEXT("projectile hit -> %s"), *OtherActor->GetActorNameOrLabel());
    //}

    // applying damage 
    // step 1 - get the hold of the owner of the projectile

    AActor* MyOwner = GetOwner();
    if (MyOwner){
        if (OtherActor && (OtherActor!=MyOwner) && (OtherActor != this)) {
            // step 2 - do research on apply damage function in unreal documentation  (inside UGamePlayStatistics)
            UGameplayStatics::ApplyDamage(
                OtherActor,
                Damage,
                MyOwner->GetInstigatorController(),
                this,
                UDamageType::StaticClass()
            );
        }
    }
    Destroy(); // we will destroy the projectile after it hit something 
}


