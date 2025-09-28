// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_OnDragSlot.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_OnDragSlot : public UUserWidget
{
	GENERATED_BODY()

public:

		
	// ================================
	// =        PROPERTIES         =
	// =================================
		
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY()
	UTexture2D* SlotIcon;
	
	// ================================
	// =         FUNCTIONS            =
	// =================================


protected:

	virtual void NativeConstruct() override;
};
