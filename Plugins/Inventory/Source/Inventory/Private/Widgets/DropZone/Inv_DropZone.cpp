// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DropZone/Inv_DropZone.h"
#include "Components/InventoryComponent.h"
#include "Widgets/Drag and Drop/Inv_DragAndDrop.h"


void UInv_DropZone::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = GetOwningPlayer();
	if (APawn* PlayerPawn = PlayerController->GetPawn())
	{
		PlayerInventory = PlayerPawn->FindComponentByClass<UInventoryComponent>();
	}
}

bool UInv_DropZone::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInv_DragAndDrop* DragDrop = Cast<UInv_DragAndDrop>(InOperation))
	{
		if (DragDrop)
		{					
			if (PlayerInventory)
				{
					PlayerInventory->RemoveItem(DragDrop->DraggedItemIndex);
				}
		}
	}

	return true;
}


