
#include "Widgets/HUD/InventoryHUD.h"

#include "Components/InventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"

#include "Widgets/DragDrop/Inv_OnDragSlot.h"
#include "Widgets/SplitStack/Inv_SplitStack.h"
#include "Widgets/Item/Slots/Inv_ItemSlot.h"
#include "Widgets/Interaction/Inv_ItemInteractor.h"
#include "Widgets/Item/Inspection/Inv_ItemInspector.h"
#include "Widgets/DragDrop/Inv_DragDrop.h"
#include "Widgets/DropZone/Inv_DropZone.h"
#include "Widgets/Item/ContextMenu/Inv_SlotContextMenu.h"



void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateDefaultWidgets();
	InventorySlots.Empty();
	DropZoneWidget->GetOnItemDropped().BindUObject(this, &UInventoryHUD::OnItemDropped);
	InventoryFilter = EItemType::None;
}

#pragma region InventoryUpdates
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

void UInventoryHUD::SetPlayerInventory(UInventoryComponent* Inventory)
{
	if (!Inventory) return;
	PlayerInventory = Inventory;
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
                			
			if (!NewSlot && !DragVisualWidget) return;

			SetSlotSetup(NewSlot, Icon, Quantity, IndexToCreate);
                				
			InventorySlots[IndexToCreate] = NewSlot;
				
			InventoryGridPanel->AddChildToGrid(NewSlot);
		}
	}
	FilterInventory(InventoryFilter);
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

	if (!SlotToInsert || !DragVisualWidget || !InventoryGridPanel) return;

	SetSlotSetup(SlotToInsert, Icon, Quantity, IndexToInsert);
	
	InventorySlots.Insert(SlotToInsert, IndexToInsert);
	
	UpdateExistingSlot({ExistingIndex});
	
	InventoryGridPanel->AddChildToGrid(SlotToInsert);

	FilterInventory(InventoryFilter);
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

		FilterInventory(InventoryFilter);
	}
}

void UInventoryHUD::OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex) const
{
	if (!PlayerInventory || !InOperation) return;

	if (UInv_DragDrop* DragOperation = Cast<UInv_DragDrop>(InOperation))
	{
		int32 DraggedIndex = DragOperation->DraggedIndex;
		
		if (DestinationIndex != INDEX_NONE) //destination index will only be valid if trying to swap item
		{
			PlayerInventory->SwapItem(DraggedIndex, DestinationIndex);
		}
		else //if destination index is not valid, then it's trying to remove the item
		{
			PlayerInventory->RemoveItem(DraggedIndex);
		}
	}
}

#pragma endregion

#pragma region CreateWidgets

void UInventoryHUD::CreateDefaultWidgets()
{
	CreateInteractWidget();
	CreateItemInspectorWidget();
	CreateDragDropSetup();
	CreateContextMenu();
}

void UInventoryHUD::CreateInteractWidget()
{
	InteractWidget = CreateWidget<UInv_ItemInteractor>(GetWorld(), InteractWidgetClass);
	
	if (!InteractWidget) return;
	
	InteractWidget->AddToViewport(1);
}

void UInventoryHUD::CreateSplitStackWidget(int32 Index)
{
	if (!SplitStackClass || !PlayerInventory) return;

	if (SplitStackWidget)
	{
		SplitStackWidget->RemoveFromParent();
		SplitStackWidget = nullptr;
	}
	
	SplitStackWidget = CreateWidget<UInv_SplitStack>(GetWorld(), SplitStackClass);
	
	if (!SplitStackWidget) return;
	
	FItemData& Item = PlayerInventory->Inventory[Index];
	
	SplitStackWidget->SlotIndex = Index;
	SplitStackWidget->MaxStack = Item.ItemNumericData.Quantity;
	SplitStackWidget->OnSplitConfirmed.BindUObject(PlayerInventory, &UInventoryComponent::SplitItem);
	SplitStackWidget->OnDropConfirmed.BindUObject(PlayerInventory, &UInventoryComponent::DropItemQuantity);
	SplitStackWidget->AddToViewport();
}

UDragDropOperation* UInventoryHUD::CreateDragDropWidget(UInv_ItemSlot* ItemSlot)
{
	if (!PlayerInventory || !DragSlotClass || !DragDropWidget) return nullptr;
	
	//set drag visual
	DragVisualWidget->SlotIcon = ItemSlot->GetSlotIcon();
	
	//set drag drop
	DragDropWidget->DefaultDragVisual = DragVisualWidget;
	DragDropWidget->DraggedIndex = ItemSlot->GetSlotIndex();
	
	return DragDropWidget;
}

void UInventoryHUD::CreateItemInspectorWidget()
{
	ItemInspector = CreateWidget<UInv_ItemInspector>(GetWorld(), ItemInspectorClass);
	
	if (!ItemInspector) return;
	
	ItemInspector->AddToViewport();
	ItemInspector->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryHUD::SetInspectorSetup(int32 ItemIndex)
{
	FItemData& ItemInfo = PlayerInventory->Inventory[ItemIndex];

	UTexture2D* ItemImage = ItemInfo.ItemAssetData.Icon.LoadSynchronous();
	FText ItemName = ItemInfo.ItemTextData.Name;
	FText ItemDescription = ItemInfo.ItemTextData.Description;
	FText ItemRarity = EnumToText(ItemInfo.ItemRarity);
	FText ItemType = EnumToText(ItemInfo.ItemType);

	if (!ItemInspector) return;
	
	ItemInspector->SetInspectorInfos(ItemImage, ItemName, ItemDescription, ItemRarity, ItemType);
	ItemInspector->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryHUD::CreateDragDropSetup()
{
	if (!PlayerInventory || !DragSlotClass || !DragDropClass) return;
	
	DragVisualWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
	DragDropWidget = NewObject<UInv_DragDrop>(GetWorld(), DragDropClass);
}

void UInventoryHUD::CreateContextMenu()
{
	
	if (!ContextMenuWidget) return;
	
	ContextMenuWidget->OnSplitClicked.BindUObject(this, &UInventoryHUD::CreateSplitStackWidget);
	ContextMenuWidget->OnDropClicked.BindUObject(PlayerInventory, &UInventoryComponent::RemoveItem);
}

void UInventoryHUD::SetContextMenuSetup(int32 SlotIndex)
{
	if (!ContextMenuWidget || SlotIndex < 0) return;
	ContextMenuWidget->SlotIndex = SlotIndex;
	if (ContextMenuWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		ContextMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ContextMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

void UInventoryHUD::SetSlotSetup(UInv_ItemSlot* ItemSlot, UTexture2D* Icon, int32 Quantity, int32 Index)
{
	if (!ItemSlot) return;
	ItemSlot->SetSlotInfo(Icon, Quantity, Index);
	ItemSlot->OnItemDropped.BindUObject(this, &UInventoryHUD::OnItemDropped);
	ItemSlot->OnSplitStart.BindUObject(this, &UInventoryHUD::CreateSplitStackWidget);
	ItemSlot->OnItemHovered.BindUObject(this, &UInventoryHUD::SetInspectorSetup);
	ItemSlot->OnHoverEnd.BindUObject(ItemInspector, &UInv_ItemInspector::HideInspector);
	ItemSlot->OnItemDragged.BindUObject(this, &UInventoryHUD::CreateDragDropWidget);
	ItemSlot->OnSlotClicked.BindUObject(this, &UInventoryHUD::SetContextMenuSetup);
	ItemSlot->OnSlotDragged.BindUObject(ContextMenuWidget, &UInv_SlotContextMenu::CloseContextMenu);
}

#pragma endregion

void UInventoryHUD::FilterInventory(EItemType FilterType)
{
	if (!InventoryGridPanel || InventorySlots.Num() <= 0 || !PlayerInventory) return;

	//create an index to any visible slot that will be used to reorganize the slot position on grid
	int32 VisibleSlotIndex = 0;
	
	for (int32 Index = 0; Index < InventorySlots.Num(); Index++)
	{
		UInv_ItemSlot* SlotToFilter = InventorySlots[Index];
		
		if (!SlotToFilter || !PlayerInventory->Inventory.IsValidIndex(Index)) continue;

		const FItemData& Item = PlayerInventory->Inventory[Index];
		
		const bool bShouldBeVisible = (FilterType == EItemType::None || Item.ItemType == FilterType);
		
		if (bShouldBeVisible)
		{
			SlotToFilter->SetVisibility(ESlateVisibility::Visible);

			//get the slot from grid to change position
			if (UGridSlot* GridSlot = Cast<UGridSlot>(SlotToFilter->Slot))
			{
				//change the position of the slot based in if should be visible or not, avoiding empty spaces of hidden slots
				FIntPoint SlotPosition = GetGridPosition(VisibleSlotIndex);
				
				GridSlot->SetRow(SlotPosition.Y);
				GridSlot->SetColumn(SlotPosition.X);
			}
			VisibleSlotIndex++;
		}
		else
		{
			SlotToFilter->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	InventoryFilter = FilterType;
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

