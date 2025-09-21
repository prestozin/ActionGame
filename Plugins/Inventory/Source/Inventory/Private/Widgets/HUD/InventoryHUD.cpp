// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InventoryHUD.h"
#include "Widgets/Item/Inv_ItemSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"



void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();

	//start with inventory empty
	InventorySlots.Empty();
	
}



void UInventoryHUD::CreateItemSlot(int32 ItemIndex)
{
	
	if (InventoryComponent->Inventory.IsValidIndex(ItemIndex))
	{
		const FItemData& Item = InventoryComponent->Inventory[ItemIndex];
		UTexture2D* Icon = Item.ItemAssetData.Icon.LoadSynchronous();;
		int32 Quantity = Item.ItemNumericData.Quantity;;
		
				       
		NewSlot = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
		if (InventorySlots.Num() <= ItemIndex)
		{
			InventorySlots.SetNum(ItemIndex + 1);
		}
		
		InventorySlots[ItemIndex] = NewSlot;
				
		NewSlot->SetSlotInfo(Icon, Quantity, ItemIndex);
		NewSlot->GetInventory(InventoryComponent);
		InventoryWrapBox->AddChildToWrapBox(NewSlot);
	}
}

void UInventoryHUD::UpdateHud(int32 ItemIndex)
{
	
		if (!InventoryComponent->Inventory.IsValidIndex(ItemIndex)) return;
		if (!InventorySlots.IsValidIndex(ItemIndex)) return;
		
		const FItemData& ItemToUpdate = InventoryComponent->Inventory[ItemIndex];
			
		UTexture2D* Icon = ItemToUpdate.ItemAssetData.Icon.LoadSynchronous();
		int32 Quantity = ItemToUpdate.ItemNumericData.Quantity;
	
		InventorySlots[ItemIndex]->SetSlotInfo(Icon, Quantity, ItemIndex);
		
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


void UInventoryHUD::GetInventoryComponent(UInventoryComponent* PlayerInventory)
{
	InventoryComponent = PlayerInventory;
}



