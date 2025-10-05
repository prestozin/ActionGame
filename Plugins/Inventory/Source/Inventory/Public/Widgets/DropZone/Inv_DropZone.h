// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_DropZone.generated.h"

#pragma region Delegates

DECLARE_DELEGATE_TwoParams(FOnDropZoneItemDropped, UDragDropOperation*, int32);

#pragma endregion

UCLASS()



class INVENTORY_API UInv_DropZone : public UUserWidget
{
	GENERATED_BODY()

private:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	FOnDropZoneItemDropped OnItemDropped;
	int32 NullIndex = INDEX_NONE;  //Only to use one function from inventory hud for 2 delegates (dropzone and item slot)
	
public:
	
	FOnDropZoneItemDropped& GetOnItemDropped() { return OnItemDropped; }
	
protected:

	virtual void NativeConstruct() override;
};
