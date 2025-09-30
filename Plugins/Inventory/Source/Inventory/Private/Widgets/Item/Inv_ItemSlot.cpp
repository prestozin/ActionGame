// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Item/Inv_ItemSlot.h"

#include "Components/InventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/DragDrop/Inv_DragDrop.h"
#include "Widgets/DragDrop/Inv_OnDragSlot.h"
#include "Widgets/SplitStack//Inv_SplitStack.h"

void UInv_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInv_ItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
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
			SplitStackWidget = CreateWidget<UInv_SplitStack>(GetWorld(), SplitStackClass);
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

bool UInv_ItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation)
{
	if (!InOperation) return false;
	
	UInv_DragDrop* DragAndDrop = Cast<UInv_DragDrop>(InOperation);

	int32 DraggedIndex = DragAndDrop->DraggedItemIndex;

	if (PlayerInventory && SlotIndex != DraggedIndex)
	{
		PlayerInventory->SwapItemSlot(DraggedIndex, SlotIndex);
	}
	return true;
}



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

