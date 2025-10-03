// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemSlot.generated.h"


class UInv_SplitStack;
class UInventoryComponent;
class UTextBlock;
class UImage;
class UInv_OnDragSlot;
class UInv_ItemInspection;


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
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SplitStack> SplitStackClass;

	//convert enum to ftext
	template<typename Enums>
	FText EnumToText(Enums EnumValue)
	{
		if (const UEnum* EnumPtr = StaticEnum<Enums>())
		{
			return EnumPtr->GetDisplayNameTextByValue((int64)EnumValue);
		}
		return FText::FromString("Invalid");
	}

	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	
	// ================================
	// =        PROPERTIES         =
	// =================================
	
	UPROPERTY ()
	UInventoryComponent* PlayerInventory;
	
	UPROPERTY(VisibleAnywhere)
	int32 DraggedSlotIndex;

	UPROPERTY()
	UInv_OnDragSlot* OnDragVisual;

	UPROPERTY()
	UInv_ItemInspection* ItemInspector;
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	UFUNCTION()
	void SetSlotInfo (UTexture2D* Icon, int32 Quantity, int32 Index);
	
	void SetSlotIndex (int32 Index);


protected:

	virtual void NativeConstruct() override;

	
	
};
