
#include "Subsystems/UIManagerSubsystem.h"

#include "GameInstance/MyGameInstance.h"
#include "Widgets/MainHud.h"
#include "Widgets/HUD/InventoryHUD.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("UIManagerSubsystem initialized."));
}

void UUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	MainHud = nullptr;
}

void UUIManagerSubsystem::OnPlayerControllerReady() 
{
	SetDefaults();
}

void UUIManagerSubsystem::SetDefaults()
{
	UWorld* GameWorld = GetWorld();
	if (!GameWorld) return;
	World = GameWorld;
	
	APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(World);
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
	if (!PlayerController || !GameInstance) return;
	
	OwningPlayer = PlayerController;
	MyGameInstance = GameInstance;
}

void UUIManagerSubsystem::CreateHUD()
{
	if (MyGameInstance->HUDClass)
	{
		MainHud = CreateWidgetOfClass(MyGameInstance->HUDClass);
		if (MainHud)
		{
			MainHud->AddToViewport();
		}
	}
}

void UUIManagerSubsystem::CreateInventoryWidget()
{
	if (MyGameInstance->InventoryClass)
	{
		InventoryHUD = CreateWidgetOfClass(MyGameInstance->InventoryClass);
		if (InventoryHUD)
		{
			InventoryHUD->AddToViewport();
		}
	}
}

void UUIManagerSubsystem::SetInventoryWidget(TObjectPtr<UObject> InventorySource)
{
	if (!InventoryHUD) return;
	InventoryHUD->InitializeInventory(InventorySource);
}

void UUIManagerSubsystem::ToggleInventoryWidget() const
{
	if (!InventoryHUD || !OwningPlayer) return;

	if (InventoryHUD->IsVisible())
	{
		InventoryHUD->SetVisibility(ESlateVisibility::Collapsed);
		OwningPlayer->SetShowMouseCursor(false);
	}
	else
	{
		InventoryHUD->SetVisibility(ESlateVisibility::Visible);
		OwningPlayer->SetShowMouseCursor(true);
	}
}


