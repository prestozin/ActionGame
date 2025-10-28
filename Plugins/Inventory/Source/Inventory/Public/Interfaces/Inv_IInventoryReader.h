// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Inv_ItemDataStructs.h"
#include "UObject/Interface.h"
#include "Inv_IInventoryReader.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_IInventoryReader : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_IInventoryReader
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	int32 GetInventorySize();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	int32 GetSlotQuantity(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	UTexture2D* GetSlotIcon(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	FText GetSlotName(int32 Index);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	FText GetSlotDescription(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	EItemRarity GetSlotRarity(int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	EItemType GetSlotType(int32 Index);
	
};
