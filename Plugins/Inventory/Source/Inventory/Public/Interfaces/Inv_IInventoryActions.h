// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_IInventoryActions.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_IInventoryActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_IInventoryActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void ISwapItem(int32 DraggedIndex, int32 DestinationIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void IRemoveItem(int32 IndexToRemove);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void ISplitItem (int32 IndexToSplit, int32 QuantityToSplit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory|Actions")
	void IDropItemQuantity (int32 ItemIndex, int32 QuantityToDrop);
};
