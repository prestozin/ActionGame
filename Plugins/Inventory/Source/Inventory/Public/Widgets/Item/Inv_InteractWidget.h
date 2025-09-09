// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_InteractWidget.generated.h"

class UOverlay;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InteractWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FString Message;
	
public:
	
	// ================================
	// =        FUNCTIONS           =
	// ================================
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowInteractMessage (const FString& InteractMessage);

	UFUNCTION(	BlueprintImplementableEvent, Category = "Inventory")
	void HideInteractMessage ();

	FString GetPickupMessage() const {return Message;}
};
