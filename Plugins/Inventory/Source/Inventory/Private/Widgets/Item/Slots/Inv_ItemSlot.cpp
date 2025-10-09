// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Item//Slots/Inv_ItemSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInv_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

#pragma region MouseActions

void UInv_ItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (OnItemDragged.IsBound())
	{
		OutOperation = OnItemDragged.Execute(this);
		OnSlotDragged.ExecuteIfBound();
	}
}

FReply UInv_ItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InMouseEvent.IsControlDown())
		{
			OnSplitStart.ExecuteIfBound(SlotIndex);
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInv_ItemSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlotClicked.ExecuteIfBound(SlotIndex);
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UInv_ItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	OnItemHovered.ExecuteIfBound(SlotIndex);
}

void UInv_ItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnHoverEnd.Execute();
}

bool UInv_ItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	OnItemDropped.ExecuteIfBound(InOperation, SlotIndex);
	return false;
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

		SetSlotIndex(Index);
	}
}

void UInv_ItemSlot::SetSlotIndex(int32 Index)
{
	if (Index >= 0)
	{
		SlotIndex = Index;
	}
}

UTexture2D* UInv_ItemSlot::GetSlotIcon()
{
	if (!SlotIcon) return nullptr;
	return SlotIcon;
}

int32 UInv_ItemSlot::GetSlotIndex()
{
	return SlotIndex;
}
