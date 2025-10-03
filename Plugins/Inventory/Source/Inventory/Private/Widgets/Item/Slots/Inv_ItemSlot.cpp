// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Item//Slots/Inv_ItemSlot.h"

#include "Components/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/DragDrop/Inv_DragDrop.h"
#include "Widgets/DragDrop/Inv_OnDragSlot.h"
#include "Widgets/SplitStack//Inv_SplitStack.h"
#include "Widgets/Item/Inspection/Inv_ItemInspection.h"

void UInv_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}


#pragma region MouseActions
void UInv_ItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UInv_DragDrop* DragDrop = NewObject<UInv_DragDrop>();
	
	if (OnDragVisual)
	{
		DragDrop->DefaultDragVisual = OnDragVisual;
		OnDragVisual->SlotIcon = SlotIcon;
		DragDrop->DraggedItemIndex = SlotIndex;
		DragDrop->DefaultDragVisual = OnDragVisual;
		OutOperation = DragDrop;
	}
}

FReply UInv_ItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InMouseEvent.IsControlDown())
		{
			UInv_SplitStack* SplitStackWidget = CreateWidget<UInv_SplitStack>(GetWorld(), SplitStackClass);
			if (SplitStackWidget)
			{
				SplitStackWidget->SlotIndex = SlotIndex;
				SplitStackWidget->PlayerInventory = PlayerInventory;
				SplitStackWidget->AddToViewport();
			}
			return FReply::Handled();
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInv_ItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (!ItemInspector) return;
	
	FItemData& ItemInfo = PlayerInventory->Inventory[SlotIndex];

	UTexture2D* ItemImage = ItemInfo.ItemAssetData.Icon.LoadSynchronous();
	FText ItemName = ItemInfo.ItemTextData.Name;
	FText ItemDescription = ItemInfo.ItemTextData.Description;
	FText ItemRarity = EnumToText(ItemInfo.ItemRarity);
	FText ItemType = EnumToText(ItemInfo.ItemType);
	
	ItemInspector->SetInspectionInfos(ItemImage, ItemName, ItemType, ItemRarity, ItemDescription);
	ItemInspector->SetVisibility(ESlateVisibility::Visible);
    	
}

void UInv_ItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (!ItemInspector) return;
	ItemInspector->SetVisibility(ESlateVisibility::Collapsed);
}

bool UInv_ItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!InOperation) return false;
	
	UInv_DragDrop* DragAndDrop = Cast<UInv_DragDrop>(InOperation);

	int32 DraggedIndex = DragAndDrop->DraggedItemIndex;

	if (PlayerInventory && SlotIndex != DraggedIndex)
	{
		PlayerInventory->SwapItem(DraggedIndex, SlotIndex);
	}
	return true;
}

#pragma endregion

void UInv_ItemSlot::SetSlotInfo(UTexture2D* Icon, int32 Quantity, int32 Index)
{
	if (Icon && Quantity)
	{
		if (Icon != SlotIcon)
		{
			SlotIcon = Icon;
			ImageIcon->SetBrushFromTexture(SlotIcon);
		}
		
		if (Quantity != SlotQuantity)
		{
			SlotQuantity = Quantity;
			TextItemQuantity->SetText(FText::AsNumber(SlotQuantity));
		}

		if (Index >= 0)
		{
			SlotIndex = Index;
		}
	}
}

void UInv_ItemSlot::SetSlotIndex(int32 Index)
{
	if (Index >= 0)
	{
		SlotIndex = Index;
	}
}

