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

protected:
	
	AInv_MasterItem();
	
	virtual void BeginPlay() override;
	
private:

	// ================================
	// =        PROPERTIES            =
	// ===============================

	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	UDataTable* DataTable;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	// ================================
	// =        FUNCTIONS           =
	// ===============================
	
	void SetItemMesh() const;
	
public:

	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY (EditAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;
    
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemStaticMesh;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ItemSkeletalMesh;
	
	// ================================
	// =        FUNCTIONS             =
	// ================================
	
	virtual void GetItemData_Implementation(FName& OutItemName, int32& OutQuantity) override;
	
};
