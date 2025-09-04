
#include "Inventory/InventoryComponent.h"
#include "Data/Inv_ItemDataStructs.h"





UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryComponent::AddItem(FName RowName, int32 Quantity)
{
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));
	if (Item)
	{
		
		Inventory.Add(*Item);
		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
		
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s não encontrado na DataTable!"), *RowName.ToString());
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}



