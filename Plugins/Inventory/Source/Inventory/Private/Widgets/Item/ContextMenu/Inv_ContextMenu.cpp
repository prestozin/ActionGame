// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Item/ContextMenu/Inv_ContextMenu.h"

#include "Components/Button.h"
#include "Editor/PropertyEditorTestObject.h"

void UInv_ContextMenu::NativeConstruct()
{
	Super::NativeConstruct();

	EquipButton->OnClicked.AddDynamic(this,&UInv_ContextMenu::EquipItem);
	SplitButton->OnClicked.AddDynamic(this,&UInv_ContextMenu::OpenSplitWidget);
	DropButton->OnClicked.AddDynamic(this,&UInv_ContextMenu::DropItem);
	CancelButton->OnClicked.AddDynamic(this,&UInv_ContextMenu::ToggleVisibility);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ContextMenu::EquipItem()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnEquipClicked.ExecuteIfBound(SlotIndex);
	}
}

void UInv_ContextMenu::OpenSplitWidget()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnSplitClicked.ExecuteIfBound(SlotIndex);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_ContextMenu::DropItem()
{
	if (SlotIndex != INDEX_NONE)
	{
		OnDropClicked.ExecuteIfBound(SlotIndex);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_ContextMenu::SetContextSetup(int32 Index)
{
	if (SlotIndex < 0)
	{
		SlotIndex = Index;
	}
	ToggleVisibility();
}

FVector2D UInv_ContextMenu::GetWidgetOffset() const
{
	return WidgetOffset;
}

void UInv_ContextMenu::ToggleVisibility()
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
