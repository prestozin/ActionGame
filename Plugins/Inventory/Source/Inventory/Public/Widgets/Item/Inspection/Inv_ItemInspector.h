// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemInspector.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class INVENTORY_API UInv_ItemInspector : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	virtual void NativeConstruct() override;
	
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

	UPROPERTY(EditAnywhere)
	FVector2D WidgetOffset;

	

public:

	void SetInspectorSetup(UTexture2D* Image, const FText& ItemName, const FText& ItemType, const FText& ItemRarity, const FText& ItemDescription) const;

	void HideInspector();

	FVector2D GetWidgetOffset() const;
};
