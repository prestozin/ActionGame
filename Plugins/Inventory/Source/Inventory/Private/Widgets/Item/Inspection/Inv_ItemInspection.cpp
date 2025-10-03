// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Item/Inspection/Inv_ItemInspection.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInv_ItemInspection::SetInspectionInfos(UTexture2D* Image, const FText& ItemName, const FText& ItemType, const FText& ItemRarity, const FText& ItemDescription)
{
	if (Image)
	{
		UTexture2D* CurrentTexture = Cast<UTexture2D>(ItemImage->GetBrush().GetResourceObject());
		
		if (CurrentTexture != Image)
		{
			ItemImage->SetBrushFromTexture(Image);
		}
	}

	if (TextItemName && !TextItemName->GetText().EqualTo(ItemName))
	{
		TextItemName->SetText(ItemName);
	}

	if (TextItemType && !TextItemType->GetText().EqualTo(ItemType))
	{
		TextItemType->SetText(ItemType);
	}

	if (TextItemRarity && !TextItemRarity->GetText().EqualTo(ItemRarity))
	{
		TextItemRarity->SetText(ItemRarity);
	}

	if (TextItemDescription && !TextItemDescription->GetText().EqualTo(ItemDescription))
	{
		TextItemDescription->SetText(ItemDescription);
	}
}
