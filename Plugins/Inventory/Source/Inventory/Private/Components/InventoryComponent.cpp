
#include "Components/InventoryComponent.h"

#include "Widgets/HUD/InventoryHUD.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Widgets/Item/Inv_InteractWidget.h"


UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//create necessary data
	CreateDefaults();

}
void UInventoryComponent::CreateDefaults()
{
		//Get owning controller
    	OwningController = GetWorld()->GetFirstPlayerController();
	
    	//create interact widget
    	InteractWidget = CreateWidget<UInv_InteractWidget>(GetWorld(), InteractWidgetClass);

		//only add to viewport if widget is valid
    	if (InteractWidget)
    	{
    		InteractWidget->AddToViewport();
    	}

		//create hud widget
		CreateHUDWidget();
}

void UInventoryComponent::CreateHUDWidget()
{
	if (OwningController.IsValid())
	{
		InventoryHUD = CreateWidget<UInventoryHUD>(OwningController.Get(),HUDWidgetClass);
        
        	if (IsValid(InventoryHUD))
        	{
        		InventoryHUD->GetInventoryComponent(this);
        		InventoryHUD->AddToViewport();
        		InventoryHUD->SetVisibility(ESlateVisibility::Collapsed);
        	}
	}
	}



void UInventoryComponent::AddItem(FName RowName, int32 Quantity)
{
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));
	if (Item)
	{
		Item->ItemNumericData.Quantity = Quantity;
		Inventory.Add(*Item);
		InventoryHUD->CreateItemSlot();
		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
		
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s não encontrado na DataTable!"), *RowName.ToString());
	}
}

UStaticMesh* UInventoryComponent::GetItemMesh_Implementation(FName RowName)
{
	if (!DataTable)  return nullptr;
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));

	if (Item)
	{
		return Item->ItemAssetData.Mesh.LoadSynchronous();
	}
	return nullptr;
}









