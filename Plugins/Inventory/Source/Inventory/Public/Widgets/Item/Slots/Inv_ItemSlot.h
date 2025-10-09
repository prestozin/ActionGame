// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemSlot.generated.h"


#pragma region Classes

class UTextBlock;
class UImage;

#pragma endregion

#pragma region Delegates


DECLARE_DELEGATE_OneParam(FOnItemHovered, int32 /*SlotIndex*/);
DECLARE_DELEGATE_OneParam(FOnSplitStart, int32 /*SlotIndex*/);
DECLARE_DELEGATE_OneParam(FOnSlotClicked, int32 /*SlotIndex*/);
DECLARE_DELEGATE(FOnHoverEnd);
DECLARE_DELEGATE_RetVal_OneParam(UDragDropOperation*, FOnItemDragged, UInv_ItemSlot* /*SlotIndex*/);
DECLARE_DELEGATE_TwoParams(FOnItemDropped,UDragDropOperation* /*DragDrop*/, int32 /*ToIndex*/);
DECLARE_DELEGATE(FOnSlotDragged);

#pragma endregion
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
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextItemQuantity;

	UPROPERTY(meta = (BindWidget))
	UImage* ImageIcon;

	UPROPERTY()
	bool bDragDetected = false;
	// ================================
	// =        FUNCTIONS            =
	// =================================

public:
	
	// ================================
	// =        PROPERTIES         =
	// =================================
	
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	UFUNCTION()
	void SetSlotInfo (UTexture2D* Icon, int32 Quantity, int32 Index);
	
	void SetSlotIndex (int32 Index);

	UTexture2D* GetSlotIcon();

	int32 GetSlotIndex();

	// ================================
	// =        DELEGATES           =
	// =================================
	
	FOnSlotClicked OnSlotClicked;

	FOnItemHovered OnItemHovered;

	FOnSplitStart OnSplitStart;

	FOnHoverEnd OnHoverEnd;

	FOnItemDragged OnItemDragged;

	FOnItemDropped OnItemDropped;

	FOnSlotDragged OnSlotDragged;
	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
