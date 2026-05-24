// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Health = MaxHealth;

	// applying the damage taken effect with the help of delegate, binding the function to delegate

		if (GetOwner())
		{
			GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageTaken);
		}

		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld()); // this will return a gamemode base pointer which is base class type for gamemode
		if (GameMode) {
			BattleBlasterGameMode = Cast<ABattleBlasterGameMode>(GameMode); // convert generic type so that we can use it 
		}
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnDamageTaken function is called"));
	if (Damage > 0) {
		Health -= Damage;
		if (Health <= 0.0f) {
			// if the health is zero then destroy the actor i.e tower or tank => this is controlled  by GameMode , so we will call it 
			// in case of tank we will not destroy it bcz if we do then camera component will aso get destroyed which is not good , so we will simply make it invisible
			

			//GetOwner()->Destroy()

			if (BattleBlasterGameMode) {
				BattleBlasterGameMode->ActorDied(DamagedActor);
			}

		}
	}
}

