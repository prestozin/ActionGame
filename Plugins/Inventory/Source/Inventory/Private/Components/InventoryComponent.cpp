
#include "Components/InventoryComponent.h"
#include "Widgets/HUD/InventoryHUD.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Items/Inv_MasterItem.h"

#pragma region StartSection

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
    	OwningController = GetWorld()->GetFirstPlayerController();
		CreateHUDWidget();
}

void UInventoryComponent::CreateHUDWidget()
{
	if (OwningController.IsValid())
	{
		InventoryHUD = CreateWidget<UInventoryHUD>(OwningController.Get(),HUDWidgetClass);
		
		if (!InventoryHUD) return;
		
		InventoryHUD->PlayerInventory = this;
		InventoryHUD->AddToViewport();
		InventoryHUD->SetVisibility(ESlateVisibility::Collapsed);
	}
}

#pragma endregion

#pragma region InventoryFunctions

void UInventoryComponent::AddItem(FName RowName, int32 Quantity)
{
	if (!DataTable) return;
	
	FItemData* CollectedItem = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));
	
	if (!CollectedItem) return;
	
	FItemData ItemToAdd = *CollectedItem;
	
	if (ItemToAdd.ItemNumericData.IsStackable)
	{
		int32 StackQuantity = Quantity;

		//create new stacks clamped at max quantity if the picked item has more quantity than max quantity
		while (StackQuantity > 0)
		{
			int32 EntryQuantity = FMath::Min(StackQuantity, ItemToAdd.ItemNumericData.MaxQuantity);
			ItemToAdd.ItemNumericData.Quantity = EntryQuantity;
			StackOnAdd(&ItemToAdd);
			StackQuantity -= EntryQuantity;
		}
	}
	else
	{
		ItemToAdd.ItemNumericData.Quantity = Quantity;
		int32 ItemIndex = Inventory.Add(ItemToAdd);
		UpdateInventorySlot(EInventoryUpdateType::Create, ItemIndex);
	}
	UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
}

void UInventoryComponent::SplitItem(int32 IndexToSplit, int32 QuantityToSplit)
{
	if (!Inventory.IsValidIndex(IndexToSplit)) return;
	
	FItemData& ItemToSplit = Inventory[IndexToSplit];
		
	if (QuantityToSplit <= 0 || QuantityToSplit >= ItemToSplit.ItemNumericData.Quantity) return;
		
	FItemData NewStack = ItemToSplit;
		
	NewStack.ItemNumericData.Quantity = QuantityToSplit;
	ItemToSplit.ItemNumericData.Quantity -= QuantityToSplit;
		
	int32 IndexToInsert =  IndexToSplit + 1;
	Inventory.Insert(NewStack, IndexToInsert);

	UpdateInventorySlot(EInventoryUpdateType::Insert, IndexToSplit, IndexToInsert);
}

void UInventoryComponent::SwapItem(int32 DraggedIndex, int32 DestinationIndex)
{
	
	if (!Inventory.IsValidIndex(DraggedIndex) && Inventory.IsValidIndex(DestinationIndex)) return;
	if (DraggedIndex == DestinationIndex) return;
	
	if (Inventory[DraggedIndex].ItemNumericData.IsStackable && Inventory[DestinationIndex].ItemNumericData.IsStackable)
	{
		StackOnSwap(DraggedIndex, DestinationIndex);
		return;
	}
		
	Inventory.Swap(DraggedIndex, DestinationIndex);
	UpdateInventorySlot(EInventoryUpdateType::Swap, DraggedIndex, DestinationIndex);
}

void UInventoryComponent::DropItemQuantity(int32 SlotIndex, int32 QuantityToSubtract)
{
	if (!Inventory.IsValidIndex(SlotIndex)) return;

	UWorld* World = GetWorld();
	AActor* ActorOwner = GetOwner();
	FItemData& Item = Inventory[SlotIndex];
	
	if (!World || !ActorOwner || Item.ID.IsNone()) return;
	
	if (Item.ItemNumericData.Quantity == QuantityToSubtract)
	{
		RemoveItem(SlotIndex);
	}
	else if (QuantityToSubtract < Item.ItemNumericData.Quantity && QuantityToSubtract > 0)
	{
		Item.ItemNumericData.Quantity -= QuantityToSubtract;
		UpdateInventorySlot(EInventoryUpdateType::Existing, SlotIndex);
		AInv_MasterItem::SpawnItem(World, DataTable, Item.ID, QuantityToSubtract, ActorOwner->GetActorLocation(),ActorOwner);
	}
}

void UInventoryComponent::RemoveItem(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return;

	UWorld* World = GetWorld();
	AActor* ActorOwner = GetOwner();
	
	if (!World || !ActorOwner) return;

	FItemData& ItemToRemove = Inventory[Index];
	
	AInv_MasterItem::SpawnItem(World, DataTable, ItemToRemove.ID, ItemToRemove.ItemNumericData.Quantity, ActorOwner->GetActorLocation(), ActorOwner);
	Inventory.RemoveAt(Index);
	UpdateInventorySlot(EInventoryUpdateType::Remove, Index);
}


void UInventoryComponent::StackOnAdd(const FItemData* Item)
{
	if (!Item) return;
	
	FItemData ItemToStack = *Item;
	for (int32 ItemIndex = 0; ItemIndex < Inventory.Num(); ItemIndex++)
	{
		FItemData& ExistingItem = Inventory[ItemIndex];

		if (ExistingItem.ID == ItemToStack.ID && ExistingItem.ItemNumericData.IsStackable)
		{
			const int32 SpaceFree  = ExistingItem.ItemNumericData.MaxQuantity - ExistingItem.ItemNumericData.Quantity;

			if (SpaceFree > 0)
			{
				const int32 AmountToAdd = FMath::Min(SpaceFree, ItemToStack.ItemNumericData.Quantity);

				ExistingItem.ItemNumericData.Quantity += AmountToAdd;
				ItemToStack.ItemNumericData.Quantity -= AmountToAdd;

				InventoryHUD->UpdateSlots(EHUDUpdates::Existing, ItemIndex);
			}
		}
	}
	
	while (ItemToStack.ItemNumericData.Quantity > 0)
	{
		int32 NewStack = FMath::Min(ItemToStack.ItemNumericData.Quantity, ItemToStack.ItemNumericData.MaxQuantity);

		FItemData NewStackSlot = ItemToStack;
		NewStackSlot.ItemNumericData.Quantity = NewStack;

		int32 SlotIndex = Inventory.Add(NewStackSlot);
		UpdateInventorySlot(EInventoryUpdateType::Create, SlotIndex);

		ItemToStack.ItemNumericData.Quantity -= NewStack;
	}
}

void UInventoryComponent::StackOnSwap(int32 DraggedIndex, int32 DestinationIndex)
{
	if (!Inventory.IsValidIndex(DraggedIndex) && Inventory.IsValidIndex(DestinationIndex)) return;
	
	FItemData& DraggedSlot = Inventory[DraggedIndex];
	FItemData& DestinationSlot = Inventory[DestinationIndex];
		
	if (DestinationSlot.ID == DraggedSlot.ID && DestinationSlot.ItemNumericData.IsStackable)
	{
		const int32 SpaceFree = DestinationSlot.ItemNumericData.MaxQuantity - DestinationSlot.ItemNumericData.Quantity;
		const int32 AmountToAdd = FMath::Min(SpaceFree, DraggedSlot.ItemNumericData.Quantity);

		if (AmountToAdd > 0)
		{
			DestinationSlot.ItemNumericData.Quantity += AmountToAdd;
			DraggedSlot.ItemNumericData.Quantity -= AmountToAdd;
		}

		if (DraggedSlot.ItemNumericData.Quantity <= 0)
		{
			Inventory.RemoveAt(DraggedIndex);
			UpdateInventorySlot(EInventoryUpdateType::Remove, DraggedIndex);
		}
		UpdateInventorySlot(EInventoryUpdateType::Swap, DestinationIndex, DraggedIndex);
	}
}


#pragma endregion

#pragma region UpdateSection

template<typename... Indexes>	//create a list called indexes, that accept various indexes
void UInventoryComponent::UpdateInventorySlot(EInventoryUpdateType UpdateType, Indexes... ModifiedIndexes)
{
	if (!InventoryHUD) return;
	
	TArray<int32> IndexesToUpdate = { ModifiedIndexes...};
	
	switch(UpdateType)
	{
	case
		EInventoryUpdateType::Create:
		UpdateOnAdd(IndexesToUpdate);
		break;
		
	case
	EInventoryUpdateType::Insert:
		UpdateOnSplit(IndexesToUpdate);
		break;

	case
	EInventoryUpdateType::Remove:
		UpdateOnRemove(IndexesToUpdate);
		break;
		
	case
		EInventoryUpdateType::Swap:
		UpdateOnSwap(IndexesToUpdate);
		break;
		
	case
		EInventoryUpdateType::Existing:
		InventoryHUD->UpdateSlots(EHUDUpdates::Existing, IndexesToUpdate);
		break;
	}
	
	InventoryHUD->UpdateIndexes();
}

void UInventoryComponent::UpdateOnAdd(const TArray<int32>& IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{
		int32 IndexToAdd = Index;
		if (Inventory.IsValidIndex(IndexToAdd) && !InventoryHUD->InventorySlots.IsValidIndex(IndexToAdd))
		{
			InventoryHUD->UpdateSlots(EHUDUpdates::Create, IndexToAdd);
		}
	}
}

void UInventoryComponent::UpdateOnSwap(const TArray<int32>& IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{
		InventoryHUD->UpdateSlots(EHUDUpdates::Existing, Index);
	}
}

void UInventoryComponent::UpdateOnSplit(const TArray<int32>& IndexesToUpdate)
{
	if (IndexesToUpdate.Num() < 2) return;
	
	int32 IndexToInsert = FMath::Max(IndexesToUpdate[0], IndexesToUpdate[1]);
		
	if (!Inventory.IsValidIndex(IndexToInsert)) return;
	
	InventoryHUD->UpdateSlots(EHUDUpdates::Insert, IndexesToUpdate);
}

void UInventoryComponent::UpdateOnRemove(const TArray<int32>& IndexesToUpdate)
{
	//remove in ascending order to make sure that the biggest index is removed first
	for (int32 Index = IndexesToUpdate.Num() -1; Index >= 0; --Index)  
	{
		int32 IndexToRemove = IndexesToUpdate[Index];
		if (!InventoryHUD->InventorySlots.IsValidIndex(IndexToRemove)) return;

		InventoryHUD->UpdateSlots(EHUDUpdates::Remove, IndexToRemove);
	}
}

#pragma endregion



	











