// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "Inv_MasterItem.generated.h"


class UStaticMeshComponent;

UCLASS()
class INVENTORY_API AInv_MasterItem : public AActor, public IInv_InteractionInterface
{
	GENERATED_BODY()

public:
	
 AInv_MasterItem();
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	
	UPROPERTY (EditAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;
    
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName Name;
	

	
	// ================================
	// =        FUNCTIONS             =
	// ================================


virtual void GetItemData_Implementation(FName& OutItemName, int32& OutQuantity) override;
	
	
	
protected:

	virtual void BeginPlay() override;
	
private:

	// ================================
	// =        PROPERTIES            =
	// ================================
	
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	
	
};
