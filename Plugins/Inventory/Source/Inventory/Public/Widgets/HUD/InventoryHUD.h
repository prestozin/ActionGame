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
class UInv_OnDragSlot;

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
	TSubclassOf<UInv_InteractWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY()
	UInv_ItemSlot* NewSlot;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_OnDragSlot> DragSlotClass;

	
	// ================================
	// =       FUNCTIONS           =
	// ================================

	
	
	
public:
	// ================================
	// =       PROPERTIES          =
	// ================================

	UPROPERTY()
	bool bInventoryOpen;

	UPROPERTY()
	TArray<UInv_ItemSlot*> InventorySlots;

	// ================================
	// =       FUNCTIONS           =
	// ================================
	

	bool ToggleHUD();

	void GetInventoryComponent(UInventoryComponent* PlayerInventory);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void CreateItemSlot(int32 ItemIndex);

	void UpdateHud(int32 ItemIndex);
	
protected:
	
	virtual void NativeConstruct() override;
	
	
};
