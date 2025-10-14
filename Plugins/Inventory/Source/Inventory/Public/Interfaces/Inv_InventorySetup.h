// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Inv_ItemDataStructs.h"
#include "UObject/Interface.h"
#include "Inv_InventorySetup.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_InventorySetup : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_InventorySetup
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
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
