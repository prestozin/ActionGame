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

void UInventoryHUD::CreateItemSlot(int32 ItemIndex)
{
	if (InventoryComponent->Inventory.IsValidIndex(ItemIndex))
	{
		const FItemData& Item = InventoryComponent->Inventory[ItemIndex];
		UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();;
		int32 Quantity = Item.ItemNumericData.Quantity;;
				       
		NewSlot = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
		UInv_OnDragSlot* OnDragWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
		if (NewSlot)
		{
			NewSlot->PlayerInventory = InventoryComponent;
			NewSlot->OnDragVisual = OnDragWidget;
            		
            		if (InventorySlots.Num() <= ItemIndex)
            		{
            			InventorySlots.SetNum(ItemIndex + 1);
            		}
            		
            		InventorySlots[ItemIndex] = NewSlot;

					if (NewSlot)
					{
						NewSlot->SetSlotInfo(Icon, Quantity, ItemIndex);
						NewSlot->PlayerInventory = InventoryComponent;
						InventoryWrapBox->AddChildToWrapBox(NewSlot);
					}
		}
	}
}

void UInventoryHUD::UpdateSlot(int32 ItemIndex)
{
	if (!InventoryComponent || InventorySlots.Num() < 0 || !InventorySlots.IsValidIndex(ItemIndex) ) return;
	
	if (!InventoryComponent->Inventory.IsValidIndex(ItemIndex))
	{
		RemoveSlotAtIndex(ItemIndex);
		return;
	}
		
	const FItemData& ItemToUpdate = InventoryComponent->Inventory[ItemIndex];
	
	UInv_ItemSlot* SlotToUpdate = InventorySlots[ItemIndex];
	if (!SlotToUpdate) return;
	
	UTexture2D* Icon = ItemToUpdate.ItemAssetData.Icon.LoadSynchronous();
	int32 Quantity = ItemToUpdate.ItemNumericData.Quantity;
	
	SlotToUpdate->SetSlotInfo(Icon, Quantity, ItemIndex);
	UpdateIndexes();
}

void UInventoryHUD::UpdateIndexes()
{
	//for each slot that exist in inventory slots array, set the index of slot in array to be the index of the slot itself
	for (int32 Index = 0; Index < InventorySlots.Num(); Index++)
	{
		if (InventorySlots[Index])
		{
			InventorySlots[Index]->SetSlotIndex(Index);
		}
	}
}

void UInventoryHUD::RemoveSlotAtIndex(int32 IndexToRemove)
{
	if (InventorySlots.IsValidIndex(IndexToRemove))
	{
		UInv_ItemSlot* SlotToRemove = InventorySlots[IndexToRemove];
		if (SlotToRemove && InventoryWrapBox)
		{
			InventorySlots.RemoveAt(IndexToRemove);
			InventoryWrapBox->RemoveChild(SlotToRemove);
		}
		UpdateIndexes();
	}
}

void UInventoryHUD::InsertSlotAtIndex(int32 ItemIndex)
{
	
	if (UInv_ItemSlot* SlotToInsert  = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass))
	{
		UInv_OnDragSlot* OnDragWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
		
		if (OnDragWidget && InventoryComponent)
		{
			SlotToInsert->OnDragVisual = OnDragWidget;
			SlotToInsert->PlayerInventory = InventoryComponent;
		}
		
		InventorySlots.Insert(SlotToInsert, ItemIndex);
		
		if (InventoryWrapBox)
		{
			InventoryWrapBox->ClearChildren();
			for (UInv_ItemSlot* InvSlot : InventorySlots)
			{
				if (InvSlot)
				{
					InventoryWrapBox->AddChildToWrapBox(InvSlot);
				}
			}
		}
		UpdateIndexes();
	}
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

void UInventoryHUD::GetInventoryComponent(UInventoryComponent* PlayerInventory)
{
	InventoryComponent = PlayerInventory;
}



