
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

void UUIManagerSubsystem::CreateMainHUD()
{
	UWorld* World = GetWorld();
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
	
	if (!World || !LocalPlayer || !MyGameInstance || !MyGameInstance->HUDClass) return;
		
	MainHud = CreateWidget<UMainHud> (World, MyGameInstance->HUDClass);
	
	if (MainHud)
	{
		MainHud->AddToViewport();
	}
}

void UUIManagerSubsystem::SetupInventory(UObject* InventorySource)
{
	if (MainHud && InventorySource && MainHud->InventoryHUD)
	{
		MainHud->InventoryHUD->InitializeInventory(InventorySource);
	}
}
