// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Drag and Drop/Inv_OnDragSlot.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"


void UInv_OnDragSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotIcon)
	{
		ItemIcon->SetBrushFromSoftTexture(SlotIcon);
	}
}

