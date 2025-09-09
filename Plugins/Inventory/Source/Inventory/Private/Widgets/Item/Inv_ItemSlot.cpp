// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Item/Inv_ItemSlot.h"


void UInv_ItemSlot::SetSlotInfo(UTexture2D* Icon, int32 Quantity)
{
	SlotIcon = Icon;
	SlotQuantity = Quantity;
}
