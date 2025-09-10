// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InventoryHUD.h"
#include "Widgets/Item/Inv_ItemSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"



void UInventoryHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
}



UInv_ItemSlot* UInventoryHUD::CreateItemSlot()
{
	
	if (!ItemSlotClass) return nullptr;

	if (ItemSlotClass)
		{
			ClearWrapBox();
			for (const FItemData& Inventory : InventoryComponent->Inventory)
				{
						UTexture2D* Icon;
						int32 Quantity;
				
            			Icon = Inventory.ItemAssetData.Icon.LoadSynchronous();
            			Quantity = Inventory.ItemNumericData.Quantity;
            			
            			ItemSlot = CreateWidget<UInv_ItemSlot>(GetWorld(), ItemSlotClass);
            			ItemSlot->SetSlotInfo(Icon, Quantity);
						InventoryWrapBox->AddChildToWrapBox(ItemSlot);
				}
		}
		
	return ItemSlot;
}

void UInventoryHUD::ClearWrapBox() const
{
	InventoryWrapBox->ClearChildren();
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



