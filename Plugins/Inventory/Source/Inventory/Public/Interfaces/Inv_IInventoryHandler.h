// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_IInventoryHandler.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_IInventoryHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_IInventoryHandler
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void ISwapItem(int32 DraggedIndex, int32 DestinationIndex);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void ISplitItem (int32 IndexToSplit, int32 QuantityToSplit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void IDropItem (int32 ItemIndex, int32 QuantityToDrop = -1);
};
