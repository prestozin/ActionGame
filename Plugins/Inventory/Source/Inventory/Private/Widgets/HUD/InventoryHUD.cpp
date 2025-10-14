
#include "Widgets/HUD/InventoryHUD.h"

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
#include "Interfaces/Inv_InventoryActions.h"
#include "Interfaces/Inv_InventorySetup.h"
#include "Interfaces/Inv_InventoryListener.h"



void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CreateDefaultWidgets();
}

void UInventoryHUD::InitializeHUD(UObject* IntInventorySource)
{
	InventorySlots.Empty();
	InventoryFilter = EItemType::None;

	if (IntInventorySource && IntInventorySource->GetClass()->ImplementsInterface(UInv_InventorySetup::StaticClass()))
	{
		InventorySource = IntInventorySource;
		
		if (IntInventorySource->GetClass()->ImplementsInterface(UInv_InventoryActions::StaticClass()))
		{
			Execute_RegisterListener(IntInventorySource, this);
		}
	}
	
	if (!DropZoneWidget) return;
	DropZoneWidget->GetOnItemDropped().BindUObject(this, &UInventoryHUD::OnItemDropped);
	SetVisibility(ESlateVisibility::Collapsed);
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
	if (!InteractWidgetClass) return;
	InteractWidget = WidgetFactory(InteractWidgetClass);
	
	if (!InteractWidget) return;
	
	InteractWidget->AddToViewport(1);
}

void UInventoryHUD::CreateSplitStackWidget(int32 Index)
{
	if (!SplitStackClass) return;

	if (SplitStackWidget)
	{
		SplitStackWidget->RemoveFromParent();
		SplitStackWidget = nullptr;
	}
	
	SplitStackWidget = WidgetFactory(SplitStackClass);
	
	if (!SplitStackWidget) return;
	
	SplitStackWidget->SlotIndex = Index;
	SplitStackWidget->MaxStack = Execute_GetSlotQuantity(InventorySource.GetObject(), Index);
	SplitStackWidget->OnSplitConfirmed.BindLambda([this](int32 Index, int32 Quantity)
	{
		Execute_ISplitItem(InventorySource->_getUObject(), Index, Quantity);
	});
	SplitStackWidget->OnDropConfirmed.BindLambda([this](int32 Index, int32 Quantity)
	{
		Execute_IDropItemQuantity(InventorySource->_getUObject(), Index, Quantity);
	});
	SplitStackWidget->AddToViewport();
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

void UInventoryHUD::CreateItemInspectorWidget()
{
	if (!ItemInspectorClass) return;
	
	ItemInspector = WidgetFactory(ItemInspectorClass);
	
	if (!ItemInspector) return;
	
	ItemInspector->AddToViewport();
	ItemInspector->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryHUD::SetInspectorSetup(int32 ItemIndex)
{
	UTexture2D* ItemImage = Execute_GetSlotIcon(InventorySource.GetObject(), ItemIndex);
	FText ItemName = Execute_GetSlotName(InventorySource.GetObject(), ItemIndex);
	FText ItemDescription = Execute_GetSlotDescription(InventorySource.GetObject(), ItemIndex);
	FText ItemRarity = EnumToText(Execute_GetSlotRarity(InventorySource.GetObject(), ItemIndex));
	FText ItemType = EnumToText(Execute_GetSlotType(InventorySource.GetObject(), ItemIndex));

	if (!ItemInspector) return;
	
	ItemInspector->SetInspectorInfos(ItemImage, ItemName, ItemDescription, ItemRarity, ItemType);
	ItemInspector->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryHUD::CreateDragDropSetup()
{
	if (!DragSlotClass || !DragDropClass) return;
	
	DragVisualWidget = WidgetFactory(DragSlotClass);
	DragDropWidget = NewObject<UInv_DragDrop>(GetWorld(), DragDropClass);
}

void UInventoryHUD::CreateContextMenu()
{
	
	if (!ContextMenuWidget || !ContextMenuClass) return;
	
	ContextMenuWidget->OnSplitClicked.BindUObject(this, &UInventoryHUD::CreateSplitStackWidget);
	ContextMenuWidget->OnDropClicked.BindLambda([this](int32 Index)
	{
		Execute_IRemoveItem(InventorySource->_getUObject(), Index);
	});
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

