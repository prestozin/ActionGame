// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_DropZone.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()



class INVENTORY_API UInv_DropZone : public UUserWidget
{
	GENERATED_BODY()

private:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY()
	UInventoryComponent* PlayerInventory;

protected:

	virtual void NativeConstruct() override;
};
