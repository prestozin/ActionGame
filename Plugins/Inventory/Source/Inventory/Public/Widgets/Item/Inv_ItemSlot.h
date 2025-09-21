// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemSlot.generated.h"



class UInventoryComponent;
class UTextBlock;
class UImage;


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

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	UPROPERTY (BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* PlayerInventory;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemQuantity;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageIcon;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
public:
	
	// ================================
	// =        PROPERTIES         =
	// =================================
	
	UPROPERTY(VisibleAnywhere)
	int32 DraggedSlotIndex;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	

	UFUNCTION()
	void SetSlotInfo (UTexture2D* Icon, int32 Quantity, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetInventory(UInventoryComponent* Inventory);

	void SetSlotIndex (int32 Index);

	
protected:

	virtual void NativeConstruct() override;
	
};
