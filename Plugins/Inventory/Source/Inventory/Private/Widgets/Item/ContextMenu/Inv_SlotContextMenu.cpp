// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Item/ContextMenu/Inv_SlotContextMenu.h"
#include "Components/Button.h"
#include "Editor/PropertyEditorTestObject.h"

void UInv_SlotContextMenu::NativeConstruct()
{
	Super::NativeConstruct();

	EquipButton->OnClicked.AddDynamic(this,&UInv_SlotContextMenu::EquipItem);
	SplitButton->OnClicked.AddDynamic(this,&UInv_SlotContextMenu::OpenSplitWidget);
	DropButton->OnClicked.AddDynamic(this,&UInv_SlotContextMenu::DropItem);
	CancelButton->OnClicked.AddDynamic(this,&UInv_SlotContextMenu::CloseContextMenu);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_SlotContextMenu::EquipItem()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnEquipClicked.ExecuteIfBound(SlotIndex);
	}
	
}

void UInv_SlotContextMenu::OpenSplitWidget()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnSplitClicked.ExecuteIfBound(SlotIndex);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_SlotContextMenu::DropItem()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnDropClicked.ExecuteIfBound(SlotIndex);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_SlotContextMenu::CloseContextMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
