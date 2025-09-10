// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryHUD.generated.h"

class UWrapBox;
class UInventoryComponent;
class UInv_InteractWidget;
class UInv_ItemSlot;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInventoryHUD : public UUserWidget, public IInv_InteractionInterface
{
	GENERATED_BODY()
private:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_InteractWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY()
	UInv_ItemSlot* ItemSlot;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
	
	// ================================
	// =       FUNCTIONS           =
	// ================================

	
	
	
public:
	// ================================
	// =       PROPERTIES          =
	// ================================

	UPROPERTY()
	bool bInventoryOpen;

	// ================================
	// =       FUNCTIONS           =
	// ================================

	void ClearWrapBox() const;

	bool ToggleHUD();

	void GetInventoryComponent(UInventoryComponent* PlayerInventory);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	UInv_ItemSlot* CreateItemSlot();
	
protected:
	
	virtual void NativeConstruct() override;
	
	
};
