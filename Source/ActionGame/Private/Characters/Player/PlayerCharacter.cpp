// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Inv_InteractionInterface.h"

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
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSphereBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSphereEndOverlap);
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

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


#pragma region Input

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		PlayerInput->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		PlayerInput->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &APlayerCharacter::OpenInventory);
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

#pragma endregion

#pragma region Interact

void APlayerCharacter::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Item") && OtherActor->GetClass()->ImplementsInterface(UInv_InteractionInterface::StaticClass()))
	{
		NearActor = OtherActor;
		UE_LOG(LogTemp, Warning, TEXT("Item próximo: %s"), *OtherActor->GetName());
	}
	
}

void APlayerCharacter::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == NearActor)
	{
		NearActor = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("ator fora de alcance"));
	}
}

void APlayerCharacter::Interact()
{
				// se other actor for valido, tiver a tag e tiver a interface, passe para proxima etapa
			if (NearActor && NearActor->ActorHasTag("Item") && NearActor->GetClass()->ImplementsInterface(UInv_InteractionInterface::StaticClass()))
			{
				FName ItemName;
				int32 Quantity;

				// pegue a interface do other actor, e chame a função preenchida, defina de quem é a função (other actor) e preencha os inputs
				NearActor->GetClass()->ImplementsInterface(UInv_InteractionInterface::StaticClass());
				IInv_InteractionInterface::Execute_GetItemData(NearActor, ItemName, Quantity);

				UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *ItemName.ToString());

				if (PlayerInventory)
				{
					PlayerInventory->AddItem(ItemName, Quantity);

					NearActor->Destroy();
				}

				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Item e: %s não encontrado"), *ItemName.ToString());
				}
			}
}

#pragma endregion

void APlayerCharacter::OpenInventory()
{
	
}


