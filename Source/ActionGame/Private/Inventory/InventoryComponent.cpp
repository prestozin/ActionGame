


#include "Inventory/InventoryComponent.h"
#include "Data/ItemDataStructs.h"



UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	
}


void UInventoryComponent::AddItem(FName ItemName)
{
	FItemData* Item = DataTable->FindRow<FItemData>(ItemName, TEXT("GetItemFromDataTable"));
	if (Item)
	{
		
		Inventory.Add(*Item);
		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *ItemName.ToString());
		
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s não encontrado na DataTable!"), *ItemName.ToString());
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}



