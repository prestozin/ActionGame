// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DropZone/Inv_DropZone.h"

void UInv_DropZone::NativeConstruct()
{
	Super::NativeConstruct();
	
}

bool UInv_DropZone::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	OnItemDropped.ExecuteIfBound(InOperation, NullIndex);
	return true;
}


