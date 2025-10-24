
#include "Widgets/HUD/InventoryHUD.h"

#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "Widgets/DragDrop/Inv_OnDragSlot.h"
#include "Widgets/SplitStack/Inv_SplitStack.h"
#include "Widgets/Item/Slots/Inv_ItemSlot.h"
#include "Widgets/Item/Inspection/Inv_ItemInspector.h"
#include "Widgets/DragDrop/Inv_DragDrop.h"
#include "Widgets/DropZone/Inv_DropZone.h"
#include "Widgets/Item/ContextMenu/Inv_ContextMenu.h"
#include "Interfaces/Inv_IInventoryActions.h"
#include "Interfaces/Inv_IInventoryInfo.h"
#include "Interfaces/Inv_IInventoryListener.h"



void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	SetDragDrop();
	SetDropZone();
	InventorySlots.Empty();
	InventoryFilter = EItemType::None;
}

void UInventoryHUD::InitializeInventory(UObject* IntInventorySource)
{
	if (IntInventorySource && IntInventorySource->GetClass()->ImplementsInterface(UInv_IInventoryInfo::StaticClass()))
	{
		InventorySource = IntInventorySource;
		
		if (IntInventorySource->GetClass()->ImplementsInterface(UInv_IInventoryActions::StaticClass()))
		{
			Execute_RegisterListener(IntInventorySource, this);
		}
	}
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

void UInventoryHUD::UpdateExistingSlot(TArray<int32> IndexToUpdate)
{
	for (int32 Index : IndexToUpdate)
	{
		if (!InventorySlots.IsValidIndex(Index)) return;
		
		UInv_ItemSlot* SlotToUpdate = InventorySlots[Index];
		int32 SlotQuantity = Execute_GetSlotQuantity(InventorySource.GetObject(), Index);
		if ( SlotQuantity <= 0)
		{
			if (SlotToUpdate && InventoryGridPanel)
			{ RemoveSlot({Index}); } 
		} 
		else if (SlotToUpdate) 
		{
			UTexture2D* Icon = Execute_GetSlotIcon(InventorySource.GetObject(), Index);
			int32 Quantity = Execute_GetSlotQuantity(InventorySource.GetObject(), Index);
			SlotToUpdate->SetSlotInfo(Icon, Quantity, Index);
		}
	}
} 

void UInventoryHUD::CreateSlot(TArray<int32> IndexToUpdate)
{
	int32 InventorySize = Execute_GetInventorySize(InventorySource->_getUObject());
	InventorySlots.SetNum(InventorySize);
	
	for (int32 Index : IndexToUpdate)
	{
		if (InventorySlots.IsValidIndex(Index) && InventorySlots[Index] != nullptr)
		{
			UpdateExistingSlot({ Index });
		} 
		else
		{
			UTexture2D* Icon = Execute_GetSlotIcon(InventorySource.GetObject(), Index);
			int32 Quantity = Execute_GetSlotQuantity(InventorySource.GetObject(), Index);
			                        				       
			UInv_ItemSlot* NewSlot = WidgetFactory(ItemSlotClass);
                			
			if (!NewSlot && !DragVisualWidget) return;

			SetSlotSetup(NewSlot, Icon, Quantity, Index);
                				
			InventorySlots[Index] = NewSlot;
				
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
	
	UTexture2D* Icon = Execute_GetSlotIcon(InventorySource.GetObject(), IndexToInsert);
	int32 Quantity =Execute_GetSlotQuantity(InventorySource.GetObject(), IndexToInsert);
	
	UInv_ItemSlot* SlotToInsert  = WidgetFactory(ItemSlotClass);

	if (!SlotToInsert || !DragVisualWidget || !InventoryGridPanel) return;

	SetSlotSetup(SlotToInsert, Icon, Quantity, IndexToInsert);
	
	InventorySlots.Insert(SlotToInsert, IndexToInsert);
	
	UpdateExistingSlot({ExistingIndex});
	
	InventoryGridPanel->AddChildToGrid(SlotToInsert);

	FilterInventory(InventoryFilter);
}

void UInventoryHUD::RemoveSlot(TArray<int32> IndexToRemove)
{
	for (int32 Index : IndexToRemove)
	{
		if (!InventorySlots.IsValidIndex(Index)) return;
		
		UInv_ItemSlot* SlotToRemove = InventorySlots[Index];
			
		if (!SlotToRemove || !InventoryGridPanel) return;
		
		InventoryGridPanel->RemoveChild(SlotToRemove);
		InventorySlots.RemoveAt(Index);
	}
	FilterInventory(InventoryFilter);
}

void UInventoryHUD::OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex) const
{
	if (!InOperation) return;

	if (UInv_DragDrop* DragOperation = Cast<UInv_DragDrop>(InOperation))
	{
		int32 DraggedIndex = DragOperation->DraggedIndex;
		
		if (DestinationIndex != INDEX_NONE) //destination index will only be valid if trying to swap item
		{
			if (InventorySource)
			{
				Execute_ISwapItem(InventorySource.GetObject(), DraggedIndex, DestinationIndex);
			}
		}
		else //if destination index is not valid, then it's trying to remove the item
		{
			if (InventorySource)
			{
				Execute_IRemoveItem(InventorySource.GetObject(), DraggedIndex);
			}
		}
	}
}

#pragma endregion

#pragma region SetupWidgets

void UInventoryHUD::SetSplitStackWidget(int32 Index)
{
	if (!SplitStackWidget) return;
	
	SplitStackWidget->SetIndexToSplit(Index);
	SplitStackWidget->SetMaxStack(Execute_GetSlotQuantity(InventorySource.GetObject(), Index));
	SplitStackWidget->OnSplitConfirmed.BindLambda([this](int32 Index, int32 Quantity)
	{
		Execute_ISplitItem(InventorySource->_getUObject(), Index, Quantity);
	});
	SplitStackWidget->OnDropConfirmed.BindLambda([this](int32 Index, int32 Quantity)
	{
		Execute_IDropItemQuantity(InventorySource->_getUObject(), Index, Quantity);
	});
}

UDragDropOperation* UInventoryHUD::CreateDragDropWidget(UInv_ItemSlot* ItemSlot)
{
	if (!DragSlotClass || !DragDropWidget) return nullptr;
	
	//set drag visual
	DragVisualWidget->SlotIcon = ItemSlot->GetSlotIcon();
	
	//set drag drop
	DragDropWidget->DefaultDragVisual = DragVisualWidget;
	DragDropWidget->DraggedIndex = ItemSlot->GetSlotIndex();
	
	return DragDropWidget;
}

void UInventoryHUD::SetInspectorSetup(int32 ItemIndex, FVector2D SlotPosition)
{
	if (!ItemInspectorWidget) return;
	
	UTexture2D* ItemImage = Execute_GetSlotIcon(InventorySource.GetObject(), ItemIndex);
	FText ItemName = Execute_GetSlotName(InventorySource.GetObject(), ItemIndex);
	FText ItemDescription = Execute_GetSlotDescription(InventorySource.GetObject(), ItemIndex);
	FText ItemRarity = EnumToText(Execute_GetSlotRarity(InventorySource.GetObject(), ItemIndex));
	FText ItemType = EnumToText(Execute_GetSlotType(InventorySource.GetObject(), ItemIndex));
	
	ItemInspectorWidget->SetInspectorSetup(ItemImage, ItemName, ItemDescription, ItemRarity, ItemType);
	SetWidgetPosition(ItemInspectorWidget, SlotPosition, ItemInspectorWidget->GetWidgetOffset());
	ItemInspectorWidget->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryHUD::SetDragDrop()
{
	if (!DragSlotClass || !DragDropClass) return;
	
	DragVisualWidget = WidgetFactory(DragSlotClass);
	DragDropWidget = NewObject<UInv_DragDrop>(GetWorld(), DragDropClass);
}

void UInventoryHUD::SetDropZone()
{
	if (!DropZoneWidget) return;
	DropZoneWidget->GetOnItemDropped().BindUObject(this, &UInventoryHUD::OnItemDropped);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryHUD::SetContextMenu()
{
	if (!ContextMenuWidget) return;
	ContextMenuWidget->OnSplitClicked.BindUObject(this, &UInventoryHUD::SetSplitStackWidget);
	ContextMenuWidget->OnDropClicked.BindLambda([this](int32 Index)
	{
		Execute_IRemoveItem(InventorySource->_getUObject(), Index);
	});
}

void UInventoryHUD::SetContextMenuSetup(int32 SlotIndex, FVector2D SlotPosition)
{
	if (!ContextMenuWidget) return;
	ContextMenuWidget->SetContextSetup(SlotIndex);
	SetWidgetPosition(ContextMenuWidget, SlotPosition, ContextMenuWidget->GetWidgetOffset());
}

void UInventoryHUD::SetSlotSetup(UInv_ItemSlot* ItemSlot, UTexture2D* Icon, int32 Quantity, int32 Index)
{
	if (!ItemSlot) return;
	ItemSlot->SetSlotInfo(Icon, Quantity, Index);
	ItemSlot->OnItemDropped.BindUObject(this, &UInventoryHUD::OnItemDropped);
	ItemSlot->OnSplitStart.BindUObject(this, &UInventoryHUD::SetSplitStackWidget);
	ItemSlot->OnItemHovered.BindUObject(this, &UInventoryHUD::SetInspectorSetup);
	ItemSlot->OnHoverEnd.BindUObject(ItemInspectorWidget, &UInv_ItemInspector::HideInspector);
	ItemSlot->OnItemDragged.BindUObject(this, &UInventoryHUD::CreateDragDropWidget);
	ItemSlot->OnSlotClicked.BindUObject(this, &UInventoryHUD::SetContextMenuSetup);
	ItemSlot->OnSlotDragged.BindUObject(ContextMenuWidget, &UInv_ContextMenu::ToggleVisibility);
}

#pragma endregion

void UInventoryHUD::FilterInventory(EItemType FilterType)
{
	if (!InventoryGridPanel || InventorySlots.Num() <= 0) return;

	//create an index to any visible slot that will be used to reorganize the slot position on grid
	int32 VisibleSlotIndex = 0;
	
	for (int32 Index = 0; Index < InventorySlots.Num(); Index++)
	{
		UInv_ItemSlot* SlotToFilter = InventorySlots[Index];
		
		if (!SlotToFilter) continue;
		
		EItemType ItemType = Execute_GetSlotType(InventorySource.GetObject(), Index);
		const bool bShouldBeVisible = (FilterType == EItemType::None || ItemType == FilterType);
		
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

void UInventoryHUD::OnInventoryUpdate_Implementation(EInventoryUpdateType UpdateType,const TArray<int32>& ModifiedIndexes)
{
	switch (UpdateType)
	{
	case
		EInventoryUpdateType::Create:
		CreateSlot(ModifiedIndexes);
		break;
	case
		EInventoryUpdateType::Insert:
		InsertSlot(ModifiedIndexes);
		break;
	case
		EInventoryUpdateType::Remove:
		RemoveSlot(ModifiedIndexes);
		break;
	case
		EInventoryUpdateType::Update:
		UpdateExistingSlot(ModifiedIndexes);
		break;
	}
	UpdateIndexes();
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

void UInventoryHUD::SetWidgetPosition(const UUserWidget* WidgetToMove, const FVector2D& AbsolutePosition, FVector2D Offset)
{
	if (!WidgetToMove) return;
	if (UCanvasPanelSlot* WidgetSlot = Cast<UCanvasPanelSlot>(WidgetToMove->Slot))
	{
		if  (UCanvasPanel* ParentCanvas = Cast<UCanvasPanel>(WidgetSlot->Parent))
		{
			FGeometry CanvasGeometry = ParentCanvas->GetCachedGeometry();
			FVector2D CanvasSize = CanvasGeometry.GetLocalSize();
			FVector2D LocalPosition = CanvasGeometry.AbsoluteToLocal(AbsolutePosition);
			
			// fix by pivot value (0.5, 0.5f)
			LocalPosition -= CanvasSize * FVector2D(0.5f, 0.5f);
			
			FVector2D WidgetOffset = Offset;
			FVector2D FinalPosition = LocalPosition + WidgetOffset;
			
			WidgetSlot->SetPosition(FinalPosition);
		}
	}
}
