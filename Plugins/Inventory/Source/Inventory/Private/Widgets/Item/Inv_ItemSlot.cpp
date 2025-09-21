// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Item/Inv_ItemSlot.h"

#include "Components/InventoryComponent.h"
#include "Components/Image.h"
#include  "Components/TextBlock.h"
#include "Widgets/Drag and Drop/Inv_DragAndDrop.h"
#include "Widgets/Drag and Drop/Inv_OnDragSlot.h"

void UInv_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInv_ItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	UInv_DragAndDrop* DragDrop = NewObject<UInv_DragAndDrop>();
	UInv_OnDragSlot* OnDragVisual = CreateWidget<UInv_OnDragSlot>(GetWorld(), UInv_OnDragSlot::StaticClass());
	DragDrop->DefaultDragVisual = OnDragVisual;
	OnDragVisual->AddToViewport(0);
	OnDragVisual->ItemIcon->SetBrushFromTexture(SlotIcon);
	
	DragDrop->DraggedItemIndex = SlotIndex;
	DragDrop->DefaultDragVisual = OnDragVisual;
	OutOperation = DragDrop;
}

bool UInv_ItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!InOperation) return false;
	
	UInv_DragAndDrop* DragAndDrop = Cast<UInv_DragAndDrop>(InOperation);

	int32 DraggedIndex = DragAndDrop->DraggedItemIndex;

	if (SlotIndex != DraggedIndex)
	{
		PlayerInventory->SwapItemSlot(DraggedIndex, SlotIndex);
	}
	return true;
	
	
}



void UInv_ItemSlot::SetSlotInfo(UTexture2D* Icon, int32 Quantity, int32 Index)

{
		SlotIcon = Icon;
		SlotQuantity = Quantity;
		SlotIndex = Index;
	
		ImageIcon->SetBrushFromTexture(SlotIcon);
		TextItemQuantity->SetText(FText::AsNumber(SlotQuantity));
}

void UInv_ItemSlot::GetInventory(UInventoryComponent* Inventory)
{
	PlayerInventory = Inventory;
}

void UInv_ItemSlot::SetSlotIndex(int32 Index)
{
	SlotIndex = Index;
}

