// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/Inv_InventoryTypes.h"
#include "Inv_IInventoryObserver.generated.h"

UINTERFACE()

class UInv_IInventoryObserver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_IInventoryObserver
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	void OnInventoryUpdate(EInventoryUpdateType UpdateType,const TArray<int32>& ModifiedIndexes);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Inventory")
	void RegisterObserver(UObject* ObjectListener);
};
