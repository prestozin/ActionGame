#include "PlayerController/MyPlayerController.h"
#include "Subsystems/UIManagerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

#include "Widgets/HUD/InventoryHUD.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetUIManagerSubsystem();
	AddMappingContext();
	InitializeHUD();
	InitializePlayerInventory();
}

void AMyPlayerController::GetUIManagerSubsystem()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UUIManagerSubsystem* UISubsystem = LocalPlayer->GetSubsystem<UUIManagerSubsystem>())
		{
			UIManagerSubsystem = UISubsystem;
			UIManagerSubsystem->OnPlayerControllerReady();
		}
	}
}

void AMyPlayerController::AddMappingContext() const
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
				
			}
			if (MouseMappingContext)
			{
				InputSubsystem->AddMappingContext(MouseMappingContext, 0);
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(OpenInventory, ETriggerEvent::Started, this,&AMyPlayerController::OnInventoryInputPressed);
	}
}

void AMyPlayerController::InitializeHUD() const
{
	UIManagerSubsystem->CreateHUD();
}

void AMyPlayerController::InitializePlayerInventory() const
{
	UIManagerSubsystem->CreateInventoryWidget();
	
	if (IsLocalController())
	{
		ULocalPlayer* LocalPlayer = GetLocalPlayer();
		APawn* PlayerPawn = GetPawn();
		
		if (!LocalPlayer || !PlayerPawn || !UIManagerSubsystem) return;

		for (UActorComponent* InventoryComponent : PlayerPawn->GetComponents())
		{
			if (InventoryComponent->GetClass()->ImplementsInterface(UInv_IInventoryReader::StaticClass()))
			{
				UIManagerSubsystem->SetInventoryWidget(InventoryComponent);
				break;
			}
		}
	}
}

void AMyPlayerController::OnInventoryInputPressed()
{
	if (!UIManagerSubsystem) return;
	UIManagerSubsystem->ToggleInventoryWidget();
}


