// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inv_DragAndDrop.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_DragAndDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	int32 DraggedItemIndex;


protected:

	
};
