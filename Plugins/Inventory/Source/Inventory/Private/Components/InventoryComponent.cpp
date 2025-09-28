
#include "Components/InventoryComponent.h"
#include "Widgets/HUD/InventoryHUD.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Items/Inv_MasterItem.h"
#include "Widgets/Interaction/Inv_InteractWidget.h"

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

#pragma endregion

#pragma region InventoryFunctions

void UInventoryComponent::AddItem(FName RowName, int32 Quantity)
{
	if (!DataTable) return;
	
	FItemData* CollectedItem = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));

	if (CollectedItem)
	{
		FItemData ItemToAdd = *CollectedItem;
		ItemToAdd.ItemNumericData.Quantity = Quantity;
        
        	if (ItemToAdd.ItemNumericData.IsStackable && Inventory.Num() > 0)
        	{
        		StackOnAdd(&ItemToAdd);
        	}
		
        	else
        	{
        		int32 ItemIndex = Inventory.Add(ItemToAdd);
        		UpdateInventorySlot(EInventoryUpdateType::Add, ItemIndex);
        	}
        		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
        	}
}

void UInventoryComponent::StackOnAdd(const FItemData* Item)
{
	if (Item)
	{
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

					if (InventoryHUD)
					{
						InventoryHUD->UpdateSlot(ItemIndex);	
					}
				}
			}
		}
		
		while (ItemToStack.ItemNumericData.Quantity > 0)
		{
			int32 NewStack = FMath::Min(ItemToStack.ItemNumericData.Quantity, ItemToStack.ItemNumericData.MaxQuantity);

			FItemData NewStackSlot = ItemToStack;
			NewStackSlot.ItemNumericData.Quantity = NewStack;

			int32 SlotIndex = Inventory.Add(NewStackSlot);
			UpdateInventorySlot(EInventoryUpdateType::Add, SlotIndex);

			ItemToStack.ItemNumericData.Quantity -= NewStack;
		}
	}
}

void UInventoryComponent::StackOnSwap(int32 DraggedIndex, int32 DestinationIndex)
{
	if (Inventory.IsValidIndex(DraggedIndex) && Inventory.IsValidIndex(DestinationIndex))
	{
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
				
				if (InventoryHUD)
				{
					UpdateInventorySlot(EInventoryUpdateType::Swap, DraggedIndex, DestinationIndex);
				}
			}
		}

		if (DraggedSlot.ItemNumericData.Quantity <= 0)
		{
			Inventory.RemoveAt(DraggedIndex);
			UpdateInventorySlot(EInventoryUpdateType::Remove, DraggedIndex);
		}
	}
}

void UInventoryComponent::SplitStack(int32 Index, int32 QuantityToSplit)
{
	if (Inventory.IsValidIndex(Index))
	{
		FItemData& ItemToSplit = Inventory[Index];
		
		if (QuantityToSplit <= 0 || QuantityToSplit >= ItemToSplit.ItemNumericData.Quantity) return;
		
		FItemData NewStack = ItemToSplit;
		
		NewStack.ItemNumericData.Quantity = QuantityToSplit;
		ItemToSplit.ItemNumericData.Quantity -= QuantityToSplit;
		
		int32 IndexToInsert = Inventory.Insert(NewStack, Index + 1);
		if (InventoryHUD)
		{
			UpdateInventorySlot(EInventoryUpdateType::Insert, Index, IndexToInsert);
		}
	}
}

void UInventoryComponent::SwapItemSlot(int32 DraggedIndex, int32 DestinationIndex)
{
	if (Inventory.IsValidIndex(DraggedIndex) && Inventory.IsValidIndex(DestinationIndex))
	{
		if (Inventory[DraggedIndex].ItemNumericData.IsStackable && Inventory[DestinationIndex].ItemNumericData.IsStackable)
		{
			StackOnSwap(DraggedIndex, DestinationIndex);
		}
		else
		{
			Inventory.Swap(DraggedIndex, DestinationIndex);
			UpdateInventorySlot(EInventoryUpdateType::Swap, DraggedIndex, DestinationIndex);
		}
	}
}

void UInventoryComponent::RemoveItem(int32 Index)
{
	if (Inventory.IsValidIndex(Index))
	{
		Inventory.RemoveAt(Index);
		UpdateInventorySlot(EInventoryUpdateType::Remove, Index);
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
		EInventoryUpdateType::Add:
		UpdateOnAdd(IndexesToUpdate);
		break;
		
	case
	EInventoryUpdateType::Insert:
		UpdateOnInsert(IndexesToUpdate);
		break;

	case
	EInventoryUpdateType::Remove:
		UpdateOnRemove(IndexesToUpdate);
		break;
		
	case
		EInventoryUpdateType::Swap:
		UpdateOnSwap(IndexesToUpdate);
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
			InventoryHUD->CreateItemSlot(IndexToAdd);
		}
	}
}

void UInventoryComponent::UpdateOnSwap(const TArray<int32>& IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{
		InventoryHUD->UpdateSlot(Index);
	}
}

void UInventoryComponent::UpdateOnInsert(const TArray<int32>& IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{
		int32 IndexToInsert = Index;
		if (Inventory.IsValidIndex(IndexToInsert) && !InventoryHUD->InventorySlots.IsValidIndex(IndexToInsert))
		{
			//only insert between slots that already exists, verifying if it's not the first and last slot, if it is, create a new slot instead
			if (IndexToInsert > 0 && InventoryHUD->InventorySlots.IsValidIndex(IndexToInsert -1))
			{
				InventoryHUD->InsertSlotAtIndex(IndexToInsert);
			}
			else
			{
				InventoryHUD->CreateItemSlot(IndexToInsert);
			}
			InventoryHUD->UpdateSlot(IndexToInsert);
			InventoryHUD->UpdateSlot(IndexToInsert - 1);
		}
	}
}

void UInventoryComponent::UpdateOnRemove(const TArray<int32>& IndexesToUpdate)
{
	//remove in ascending order to make sure that the biggest index is removed first
	for (int32 Index = IndexesToUpdate.Num() -1; Index >= 0; --Index)  
	{
		int32 IndexToRemove = IndexesToUpdate[Index];
		if (!Inventory.IsValidIndex(IndexToRemove) && InventoryHUD->InventorySlots.IsValidIndex(IndexToRemove))
		{
			InventoryHUD->RemoveSlotAtIndex(IndexToRemove);
		}
	}
}

#pragma endregion

const void UInventoryComponent::SpawnItem(const FItemData& Item)
{	
	if (UWorld* World = GetWorld())
	{
		
		FVector SpawnLocation = GetOwner()->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParameters.Owner = GetOwner();

		if (Item.ItemClass)
		{
			AInv_MasterItem* SpawnedItem = World->SpawnActor<AInv_MasterItem>(Item.ItemClass, SpawnLocation, SpawnRotation, SpawnParameters);
			if (SpawnedItem)
			{
				SpawnedItem->ID = Item.ID;
				SpawnedItem->Quantity = Item.ItemNumericData.Quantity;
				UStaticMesh* StaticMesh = Item.ItemAssetData.StaticMesh.LoadSynchronous();
				USkeletalMesh* SkeletalMesh = Item.ItemAssetData.SkeletalMesh.LoadSynchronous();
				
				if (StaticMesh)
				{
					SpawnedItem->ItemStaticMesh->SetStaticMesh(Item.ItemAssetData.StaticMesh.LoadSynchronous());
				}

				if (SkeletalMesh)
				{
					SpawnedItem->ItemSkeletalMesh->SetSkeletalMesh(Item.ItemAssetData.SkeletalMesh.LoadSynchronous());
				}
			}
		}
	}
}

UObject* UInventoryComponent::GetItemMesh_Implementation(FName RowName)
{
	if (!DataTable)  return nullptr;
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));

	if (Item)
	{
		if (Item->ItemAssetData.StaticMesh.IsValid())
		{
			return Item->ItemAssetData.StaticMesh.LoadSynchronous();
		}
		if (Item->ItemAssetData.SkeletalMesh.IsValid())
		{
			return Item->ItemAssetData.SkeletalMesh.LoadSynchronous();
		}
	}
	return nullptr;
}

	











