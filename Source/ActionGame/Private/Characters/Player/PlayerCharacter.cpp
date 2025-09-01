// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"

#include "Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/InteractionInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//create and set interaction sphere
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSphereOverlap);
	InteractSphere->SetupAttachment(RootComponent);

	//create inventory component
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//create and set spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300;
	SpringArm->bUsePawnControlRotation = true;

	//create camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
		//Add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
			//Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
				//add input context
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}



void APlayerCharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// se other actor for valido, tiver a tag e tiver a interface, passe para proxima etapa
	if (OtherActor && OtherActor->ActorHasTag("Item") && OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		FName ItemName;
		int32 Quantity;

		// pegue a interface do other actor, e chame a função preenchida, defina de quem é a função (other actor) e preencha os inputs
		OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass());
		IInteractionInterface::Execute_GetItemData(OtherActor, ItemName, Quantity);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *ItemName.ToString());

		if (PlayerInventory)
		{
			PlayerInventory->AddItem(ItemName, Quantity);

			OtherActor->Destroy();
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item e: %s não encontrado"), *ItemName.ToString());
		}
	}
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		PlayerInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
	}
	
}
void APlayerCharacter::Move(const FInputActionValue& Value)
     {
     	const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	
     }

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		ACharacter::Jump();
	}
	else
	{
		ACharacter::StopJumping();
	}
}



