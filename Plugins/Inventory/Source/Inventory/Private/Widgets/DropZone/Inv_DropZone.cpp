// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DropZone/Inv_DropZone.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "Widgets/DragDrop/Inv_DragDrop.h"
#include "Widgets/HUD/InventoryHUD.h"


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
	if (UInv_DragDrop* DragDrop = Cast<UInv_DragDrop>(InOperation))
	{
		if (DragDrop)
		{					
			if (PlayerInventory)
				{
					PlayerInventory->SpawnItem(PlayerInventory->Inventory[DragDrop->DraggedItemIndex]);
					PlayerInventory->RemoveItem(DragDrop->DraggedItemIndex);
				}
		}
	}

	return true;
}


