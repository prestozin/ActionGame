
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


void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateDefaultWidgets();
	InventorySlots.Empty();
	DropZoneWidget->GetOnItemDropped().BindUObject(this, &UInventoryHUD::OnItemDropped);
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
                			
			if (!NewSlot && !DragVisualWidget) return;

			NewSlot->SetSlotInfo(Icon, Quantity, IndexToCreate);
			NewSlot->OnItemDropped.BindUObject(this, &UInventoryHUD::OnItemDropped);
			NewSlot->OnSplitStart.BindUObject(this, &UInventoryHUD::CreateSplitStackWidget);
			NewSlot->OnItemHovered.BindUObject(this, &UInventoryHUD::SetInspectorSetup);
			NewSlot->OnHoverEnd.BindUObject(ItemInspector, &UInv_ItemInspector::HideInspector);
			NewSlot->OnItemDragged.BindUObject(this, &UInventoryHUD::CreateDragDropWidget);
                				
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

	if (!SlotToInsert || !DragVisualWidget || !InventoryGridPanel) return;

	SlotToInsert->SetSlotInfo(Icon, Quantity, IndexToInsert);
	SlotToInsert->OnItemDropped.BindUObject(this, &UInventoryHUD::OnItemDropped);
	SlotToInsert->OnSplitStart.BindUObject(this, &UInventoryHUD::CreateSplitStackWidget);
	SlotToInsert->OnItemHovered.BindUObject(this, &UInventoryHUD::SetInspectorSetup);
	SlotToInsert->OnHoverEnd.BindUObject(ItemInspector, &UInv_ItemInspector::HideInspector);
	SlotToInsert->OnItemDragged.BindUObject(this, &UInventoryHUD::CreateDragDropWidget);
	
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

void UInventoryHUD::CreateDefaultWidgets()
{
	CreateInteractWidget();
	CreateItemInspectorWidget();
	CreateDragDropSetup();
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

void UInventoryHUD::OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex)
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
			PlayerInventory->SpawnItem(DraggedIndex);
			PlayerInventory->RemoveItem(DraggedIndex);
		}
	}
}

void UInventoryHUD::CreateInteractWidget()
{
	InteractWidget = CreateWidget<UInv_ItemInteractor>(GetWorld(), InteractWidgetClass);
	
	if (!InteractWidget) return;
	
	InteractWidget->AddToViewport();
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
	SplitStackWidget->AddToViewport();
}

void UInventoryHUD::CreateDragDropSetup()
{
	if (!PlayerInventory || !DragSlotClass || !DragDropClass) return;
	
	DragVisualWidget = CreateWidget<UInv_OnDragSlot>(GetWorld(), DragSlotClass);
	DragDropWidget = NewObject<UInv_DragDrop>(GetWorld(), DragDropClass);
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
	ItemInspector = CreateWidget<UInv_ItemInspector>(GetWorld(), ItemInspectionClass);
	
	if (!ItemInspector) return;
	
	ItemInspector->AddToViewport();
	ItemInspector->SetVisibility(ESlateVisibility::Collapsed);
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

