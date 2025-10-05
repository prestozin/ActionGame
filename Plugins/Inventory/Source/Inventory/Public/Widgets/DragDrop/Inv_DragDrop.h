// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Inv_DragDrop.generated.h"

UCLASS()
class INVENTORY_API UInv_DragDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	int32 DraggedIndex = -1;
};
