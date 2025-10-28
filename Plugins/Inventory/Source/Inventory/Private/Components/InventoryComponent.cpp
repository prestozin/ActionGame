
#include "Components/InventoryComponent.h"
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
}

void UInventoryComponent::BroadcastInventoryChanges(EInventoryUpdateType UpdateType, const TArray<int32>& ModifiedIndexes)
{
	for (const TScriptInterface<IInv_IInventoryObserver>& Listener : InventoryListeners)
	{
		if (Listener.GetInterface())
		{
			Execute_OnInventoryUpdate(Listener.GetObject(), UpdateType, ModifiedIndexes);
		}
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
	
	if (ItemToAdd.ItemNumericData.Quantity == 0) return;
	
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
		BroadcastInventoryChanges(EInventoryUpdateType::Create,{ItemIndex});
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

	BroadcastInventoryChanges(EInventoryUpdateType::Insert,{IndexToInsert, IndexToSplit});
}

void UInventoryComponent::SwapItem(int32 DraggedIndex, int32 DestinationIndex)
{
	
	if (!Inventory.IsValidIndex(DraggedIndex) || !Inventory.IsValidIndex(DestinationIndex)) return;
	if (DraggedIndex == DestinationIndex) return;
	
	if (Inventory[DraggedIndex].ItemNumericData.IsStackable && Inventory[DestinationIndex].ItemNumericData.IsStackable)
	{
		StackOnSwap(DraggedIndex, DestinationIndex);
		return;
	}
		
	Inventory.Swap(DraggedIndex, DestinationIndex);
	BroadcastInventoryChanges(EInventoryUpdateType::Update,{DraggedIndex, DestinationIndex});
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

				BroadcastInventoryChanges(EInventoryUpdateType::Create,{ItemIndex});
			}
		}
	}
	
	while (ItemToStack.ItemNumericData.Quantity > 0)
	{
		int32 NewStack = FMath::Min(ItemToStack.ItemNumericData.Quantity, ItemToStack.ItemNumericData.MaxQuantity);

		FItemData NewStackSlot = ItemToStack;
		NewStackSlot.ItemNumericData.Quantity = NewStack;

		int32 SlotIndex = Inventory.Add(NewStackSlot);
		BroadcastInventoryChanges(EInventoryUpdateType::Create,{SlotIndex});

		ItemToStack.ItemNumericData.Quantity -= NewStack;
	}
}

void UInventoryComponent::StackOnSwap(int32 DraggedIndex, int32 DestinationIndex)
{
	if (!Inventory.IsValidIndex(DraggedIndex) || !Inventory.IsValidIndex(DestinationIndex)) return;
	
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
			BroadcastInventoryChanges(EInventoryUpdateType::Remove,{DraggedIndex});
		}
		BroadcastInventoryChanges(EInventoryUpdateType::Update,{DestinationIndex,DraggedIndex});;
	}
}

void UInventoryComponent::DropItem(int32 SlotIndex, int32 QuantityToDrop)
{
	if (!Inventory.IsValidIndex(SlotIndex)) return;

	UWorld* World = GetWorld();
	AActor* ActorOwner = GetOwner();
	FItemData& Item = Inventory[SlotIndex];
	
	if (!World || !ActorOwner || Item.ID.IsNone()) return;
	
	if (Item.ItemNumericData.Quantity == QuantityToDrop || QuantityToDrop < 0)
	{
		AInv_MasterItem::SpawnItem(World, DataTable, Item.ID, Item.ItemNumericData.Quantity, ActorOwner->GetActorLocation(), ActorOwner);
		Inventory.RemoveAt(SlotIndex);
		BroadcastInventoryChanges(EInventoryUpdateType::Remove,{SlotIndex});
	}
	else if (QuantityToDrop < Item.ItemNumericData.Quantity && QuantityToDrop > 0)
	{
		Item.ItemNumericData.Quantity -= QuantityToDrop;
		BroadcastInventoryChanges(EInventoryUpdateType::Update,{SlotIndex});
		AInv_MasterItem::SpawnItem(World, DataTable, Item.ID, QuantityToDrop, ActorOwner->GetActorLocation(),ActorOwner);
	}
}

#pragma endregion

#pragma region InterfaceImplementations

void UInventoryComponent::RegisterObserver_Implementation(UObject* ObjectListener)
{
	if (!ObjectListener) return;

	auto Listener = MakeInterface<IInv_IInventoryObserver>(ObjectListener);
	if (!Listener) return;

	InventoryListeners.Add(Listener); 
}

int32 UInventoryComponent::GetSlotQuantity_Implementation (int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return 0;
	return Inventory[Index].ItemNumericData.Quantity;
}

int32 UInventoryComponent::GetInventorySize_Implementation() 
{
	return Inventory.Num();
}

UTexture2D* UInventoryComponent::GetSlotIcon_Implementation(int32 Index) 
{
	if (!Inventory.IsValidIndex(Index)) return nullptr;
	const FItemData& Item = Inventory[Index];
	return Item.ItemAssetData.Icon.Get();
}

FText UInventoryComponent::GetSlotName_Implementation(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return FText::GetEmpty();
	return Inventory[Index].ItemTextData.Name;
}

FText UInventoryComponent::GetSlotDescription_Implementation(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return FText::GetEmpty();
	return Inventory[Index].ItemTextData.Description;
}

EItemType UInventoryComponent::GetSlotType_Implementation(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return EItemType::None;
	return Inventory[Index].ItemType;
}

EItemRarity UInventoryComponent::GetSlotRarity_Implementation(int32 Index)
{
	if (!Inventory.IsValidIndex(Index)) return EItemRarity::None;
	return Inventory[Index].ItemRarity;
}

void UInventoryComponent::ISplitItem_Implementation(int32 IndexToSplit, int32 QuantityToSplit)
{
	SplitItem(IndexToSplit, QuantityToSplit);
}

void UInventoryComponent::ISwapItem_Implementation(int32 SourceIndex, int32 DestinationIndex)
{
	SwapItem(SourceIndex, DestinationIndex);
}

void UInventoryComponent::IDropItem_Implementation(int32 SlotIndex, int32 QuantityToSubtract)
{
	DropItem(SlotIndex, QuantityToSubtract);
}
#pragma endregion
