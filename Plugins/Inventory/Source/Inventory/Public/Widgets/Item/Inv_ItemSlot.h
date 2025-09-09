// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemSlot.generated.h"


/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_ItemSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	
	// ================================
	// =        PROPERTIES         =
	// =================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UTexture2D* SlotIcon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotQuantity;

public:
	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	UFUNCTION()
	void SetSlotInfo (UTexture2D* Icon, int32 Quantity);
	
};
