// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/InventoryFilters/Inv_Filter.h"

#include "Components/Button.h"
#include "Data/Inv_ItemDataStructs.h"

void UInv_Filter::NativeConstruct()
{
	Super::NativeConstruct();

	if (!FilterButton) return;
	FilterButton->OnClicked.AddDynamic(this, &UInv_Filter::OnFilterButtonClicked);
}

void UInv_Filter::OnFilterButtonClicked() 
{
	OnFilterClicked.ExecuteIfBound(FilterType);
}
