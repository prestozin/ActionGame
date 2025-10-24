
#include "Subsystems/UIManagerSubsystem.h"
#include "PlayerController/MyPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Widgets/HUD/InventoryHUD.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ConfigureInputSystem();
	InitializeMainHud();
	InitializePlayerInventory();
}

void AMyPlayerController::ConfigureInputSystem() const
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

void AMyPlayerController::InitializeMainHud() const
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UUIManagerSubsystem* UIManager = LocalPlayer->GetSubsystem<UUIManagerSubsystem>())
		{
			UIManager->CreateMainHUD();
		}
	}
}

void AMyPlayerController::InitializePlayerInventory() const
{
	if (IsLocalController())
	{
		ULocalPlayer* LocalPlayer = GetLocalPlayer();
	
		if (!LocalPlayer) return;
	
		APawn* PlayerPawn = GetPawn();
		UUIManagerSubsystem* UIManager = LocalPlayer->GetSubsystem<UUIManagerSubsystem>();
	
		if (!PlayerPawn || !UIManager ) return;
	
		for (UActorComponent* InventoryComponent : PlayerPawn->GetComponents())
		{
			if (InventoryComponent->GetClass()->ImplementsInterface(UInv_IInventoryInfo::StaticClass()))
			{
				UIManager->SetupInventory(InventoryComponent);
				break;
			}
		}
	}
}

