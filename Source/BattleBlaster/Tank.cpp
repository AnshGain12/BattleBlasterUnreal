

#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	// adding Input Mapping Context in begin play
	// in order to so this we need to first get the access of the player controller, by default from the pawn class we have variable called 'controller'
	// a controller is a class that is representing you as a player inside of the game world, it is the controller that is controlling the pawn that we have inside of the game 

	PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)/* by default Controller is of type pawn */ {
		// here we are going to get access to the , in order to so this we need to have access to a class called "ULocalPlayer"
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem * Subsystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				Subsystem->AddMappingContext(DefaultMappingContext, 0); //here 0 is proority , bcz we have just one inputmapping context , we don't need to worry about it 
			}
		}
	}

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerController) {
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility,false, HitResult);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,25.0,20, FColor::Blue,false);
		ABasePawn::RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input, special function called by unreal
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// here we have access to UInputComponent but we need UEnhancedInputComponent, so we need to cast this
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this , &ATank::MoveInput); // Bind Action (ActionAsset, TriggerEvent, Object, Function)
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}

}

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector DeltaLocation = FVector(0, 0, 0);
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);// to make speed of tank frame rate independent

	DeltaLocation.X = Speed * InputValue * DeltaTime; // if InputValue -> +ve then forward and vice versa 
	AddActorLocalOffset(DeltaLocation,true); // here true means it enables collision parameters
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
	DeltaRotation.Yaw = TurnRate * InputValue * DeltaTime;
	AddActorLocalRotation(DeltaRotation,true);
}

void ATank::HandleDestruction() {
	Super::HandleDestruction();
	//UE_LOG(LogTemp, Warning, TEXT("TANK HANDLE DESTRUCTION"));
	
	// destryoing the tank is not ideal bcz with that camera will also get destoryed leading in a weired look in the game so to prevent it we will simply make it invisible 

	SetActorHiddenInGame(true); // but doing this alone is not sufficient bcz still we can fire and take damage , to prevent it we need to stop the Tick() function as well
	SetActorTickEnabled(false); // apart from it we need to disable input as well 
	SetPlayerEnabled(false);

}

void ATank::SetPlayerEnabled(bool Enabled)
{
	if(PlayerController){
		if (Enabled) {
			EnableInput(PlayerController);
			
		}
		else {
			IsAlive = false;
			DisableInput(PlayerController);
		}
		PlayerController->bShowMouseCursor = Enabled;
	}
}
