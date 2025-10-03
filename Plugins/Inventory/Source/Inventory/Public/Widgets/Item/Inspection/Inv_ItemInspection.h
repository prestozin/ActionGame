// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemInspection.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class INVENTORY_API UInv_ItemInspection : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemType;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemRarity;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemDescription;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

public:

	void SetInspectionInfos(UTexture2D* Image, const FText& ItemName, const FText& ItemType, const FText& ItemRarity, const FText& ItemDescription);
	
};
