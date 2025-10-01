// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InventoryHUD.h"
#include "Widgets/Item/Inv_ItemSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "Widgets/DragDrop/Inv_OnDragSlot.h"

void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//start with inventory empty
	InventorySlots.Empty();	
	
}

void UInventoryHUD::UpdateIndexes()
{
	if (!PlayerInventory || !(InventorySlots.Num() > 0)) return;
	
	for (int32 Index = 0; Index < InventorySlots.Num(); Index++)
	{
		if (InventorySlots[Index])
		{
			InventorySlots[Index]->SetSlotIndex(Index);
		}
	}
}

void UInventoryHUD::UpdateExistingSlot(TArray<int32> IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{ 
		int32 IndexToUpdate = Index;

		if (!PlayerInventory) return; 
		if (!PlayerInventory->Inventory.IsValidIndex(IndexToUpdate) || !InventorySlots.IsValidIndex(IndexToUpdate)) continue;
		
		UInv_ItemSlot* SlotToUpdate = InventorySlots[IndexToUpdate];
		
		const FItemData& Item = PlayerInventory->Inventory[IndexToUpdate];
		
		if (Item.ItemNumericData.Quantity <= 0)
		{
			if (SlotToUpdate && InventoryWrapBox)
			{
			InventoryWrapBox->RemoveChild(SlotToUpdate); 
			InventorySlots.RemoveAt(IndexToUpdate); 
			SlotToUpdate->RemoveFromParent(); 
			} 
		} 
		else if (SlotToUpdate) 
		{
			UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();
			int32 Quantity = Item.ItemNumericData.Quantity;
			SlotToUpdate->SetSlotInfo(Icon, Quantity, IndexToUpdate); 
		}
	} 
}

void UInventoryHUD::CreateSlot(TArray<int32> IndexesToUpdate)
{
	for (int32 Index : IndexesToUpdate)
	{
		InventorySlots.SetNum(PlayerInventory->Inventory.Num());
		
		int32 IndexToCreate = Index;
		
		if (PlayerInventory->Inventory.IsValidIndex(IndexToCreate))
		{
			if (InventorySlots.IsValidIndex(IndexToCreate) && InventorySlots[IndexToCreate] != nullptr)
			{
				UpdateExistingSlot({ IndexToCreate });
			} 
			else
			{
				const FItemData& Item = PlayerInventory->Inventory[IndexToCreate];
				UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();;
				int32 Quantity = Item.ItemNumericData.Quantity;;
                        				       
				UInv_ItemSlot* NewSlot = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
				UInv_OnDragSlot* OnDragWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
                			
				if (NewSlot && OnDragWidget)
				{
					NewSlot->PlayerInventory = PlayerInventory;
					NewSlot->OnDragVisual = OnDragWidget;
					NewSlot->SetSlotInfo(Icon, Quantity, IndexToCreate);
                				
					InventorySlots[IndexToCreate] = NewSlot;
                
					if (InventoryWrapBox->GetChildrenCount() >= IndexToCreate)
					{
						InventoryWrapBox->InsertChildAt(IndexToCreate, NewSlot);
					}
					else
					{
						InventoryWrapBox->AddChild(NewSlot);
					}
				}
			}
		}
	}
}

void UInventoryHUD::RemoveSlot(TArray<int32> IndexesToUpdate)
{
	if (IndexesToUpdate.Num() <= 0) return;

	//reorder in ascending order to make shure that the biggest index will always be removed first
	IndexesToUpdate.Sort([](int32 A, int32 B) { return A > B; }); 
	
	for (int32 IndexToRemove : IndexesToUpdate)
	{
		if (InventorySlots.IsValidIndex(IndexToRemove))
		{
			UInv_ItemSlot* SlotToRemove = InventorySlots[IndexToRemove];
			
			if (SlotToRemove && InventoryWrapBox)
			{
				InventoryWrapBox->RemoveChild(SlotToRemove);
				InventorySlots.RemoveAt(IndexToRemove);
				SlotToRemove->RemoveFromParent();
			}
		}
	}
}

void UInventoryHUD::InsertSlot(TArray<int32> IndexesToUpdate)
{
	if (IndexesToUpdate.Num() < 2) return;

	int32 ExistingIndex = FMath::Min(IndexesToUpdate[0], IndexesToUpdate[1]);
	int32 IndexToInsert = FMath::Max(IndexesToUpdate[0], IndexesToUpdate[1]);
	
	const FItemData& Item = PlayerInventory->Inventory[IndexToInsert];
	UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();
	int32 Quantity = Item.ItemNumericData.Quantity;

	UInv_ItemSlot* SlotToInsert  = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
	UInv_OnDragSlot* OnDragWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);

	if (SlotToInsert && OnDragWidget)
	{
		SlotToInsert->PlayerInventory = PlayerInventory;
		SlotToInsert->OnDragVisual = OnDragWidget;
		SlotToInsert->SetSlotInfo(Icon, Quantity, IndexToInsert);
			
		InventorySlots.Insert(SlotToInsert, IndexToInsert);

		if (InventoryWrapBox)
		{
			InventoryWrapBox->ClearChildren();
			
			for (UInv_ItemSlot* InventorySlot : InventorySlots)
			{
				if (InventorySlot)
				{
					InventoryWrapBox->AddChild(InventorySlot);
				}
			}
		}
	}
	UpdateExistingSlot({ExistingIndex});
}

bool UInventoryHUD::ToggleHUD()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return false;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		return true;
	}
}




