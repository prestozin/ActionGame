// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InventoryHUD.h"
#include "Widgets/Item/Slots/Inv_ItemSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Widgets/DragDrop/Inv_OnDragSlot.h"
#include "Widgets/Item/Inspection/Inv_ItemInspection.h"

void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CreateItemInspector();
	InventorySlots.Empty();	
}

void UInventoryHUD::UpdateIndexes()
{
	if (!(InventorySlots.Num() > 0)) return;
	
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

		if (!PlayerInventory->Inventory.IsValidIndex(IndexToUpdate) || !InventorySlots.IsValidIndex(IndexToUpdate)) continue;
		
		UInv_ItemSlot* SlotToUpdate = InventorySlots[IndexToUpdate];
		
		const FItemData& Item = PlayerInventory->Inventory[IndexToUpdate];
		
		if (Item.ItemNumericData.Quantity <= 0)
		{
			if (SlotToUpdate && InventoryGridPanel)
			{ RemoveSlot({IndexToUpdate}); } 
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
		if (!PlayerInventory->Inventory.IsValidIndex(Index)) return;
		int32 IndexToCreate = Index;
		
		InventorySlots.SetNum(PlayerInventory->Inventory.Num());
		
		if (InventorySlots.IsValidIndex(IndexToCreate) && InventorySlots[IndexToCreate] != nullptr)
		{
			UpdateExistingSlot({ IndexToCreate });
		} 
		else
		{
			const FItemData& Item = PlayerInventory->Inventory[IndexToCreate];
			UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();;
			int32 Quantity = Item.ItemNumericData.Quantity;
			                        				       
			UInv_ItemSlot* NewSlot = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
			UInv_OnDragSlot* OnDragWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
                			
			if (!NewSlot && !OnDragWidget) return;
				
			NewSlot->PlayerInventory = PlayerInventory;
			NewSlot->OnDragVisual = OnDragWidget;
			if (ItemInspector) { NewSlot->ItemInspector = ItemInspector; }
			NewSlot->SetSlotInfo(Icon, Quantity, IndexToCreate);
                				
			InventorySlots[IndexToCreate] = NewSlot;
			
			if (!InventoryGridPanel) return;

			//slot position on grid
			FIntPoint SlotPosition = GetGridPosition(IndexToCreate);
			
			InventoryGridPanel->AddChildToGrid(NewSlot, SlotPosition.Y, SlotPosition.X);
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

	if (!SlotToInsert || !OnDragWidget || !InventoryGridPanel) return;
	
	SlotToInsert->PlayerInventory = PlayerInventory;
	SlotToInsert->OnDragVisual = OnDragWidget;
	if (ItemInspector) { SlotToInsert->ItemInspector = ItemInspector; }
	SlotToInsert->SetSlotInfo(Icon, Quantity, IndexToInsert);
			
	InventorySlots.Insert(SlotToInsert, IndexToInsert);
	
	UpdateExistingSlot({ExistingIndex});
	
	FIntPoint SlotPosition = GetGridPosition(IndexToInsert);
	InventoryGridPanel->AddChildToGrid(SlotToInsert, SlotPosition.Y, SlotPosition.X);
			
	for (int32 Index = IndexToInsert + 1; Index < InventorySlots.Num(); Index++)
	{
		if (InventorySlots[Index])
		{
			FIntPoint UpdatedPosition = GetGridPosition(Index);
			if (UGridSlot* GridSlot = Cast<UGridSlot>(InventorySlots[Index]->Slot))
			{
				GridSlot->SetRow(UpdatedPosition.Y);
				GridSlot->SetColumn(UpdatedPosition.X);
			}
			InventorySlots[Index]->SetSlotIndex(Index);
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
		if (!InventorySlots.IsValidIndex(IndexToRemove)) return;
		
		UInv_ItemSlot* SlotToRemove = InventorySlots[IndexToRemove];
			
		if (!SlotToRemove || !InventoryGridPanel) return;
		
		InventoryGridPanel->RemoveChild(SlotToRemove);
		InventorySlots.RemoveAt(IndexToRemove);
		SlotToRemove->RemoveFromParent();

		for (int32 IndexToUpdate = IndexToRemove; IndexToUpdate < InventorySlots.Num(); IndexToUpdate++)
		{
			if (InventorySlots[IndexToUpdate])
			{
				if (UGridSlot* GridSlot = Cast<UGridSlot>(InventorySlots[IndexToUpdate]->Slot))
				{
					FIntPoint SlotPosition = GetGridPosition(IndexToUpdate);
					GridSlot->SetRow(SlotPosition.Y);
					GridSlot->SetColumn(SlotPosition.X);
				}
				InventorySlots[IndexToUpdate]->SetSlotIndex(IndexToUpdate);
			}
		}
		
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

FIntPoint UInventoryHUD::GetGridPosition(int32 Index) const
{
	int32 Col = Index % SlotsPerLine;   
	int32 Row = Index / SlotsPerLine;   
	return FIntPoint(Col, Row);
}

void UInventoryHUD::CreateItemInspector()
{
	ItemInspector = CreateWidget<UInv_ItemInspection>(GetWorld(), ItemInspectionClass);
	ItemInspector->AddToViewport();
	ItemInspector->SetVisibility(ESlateVisibility::Collapsed);
}


