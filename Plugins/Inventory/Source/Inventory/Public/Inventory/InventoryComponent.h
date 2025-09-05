

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_InteractionInterface
{
	GENERATED_BODY()

public:
	
	UInventoryComponent(); //Constructor

	
	// ================================
	// =        PROPERTIES            =
	// ================================

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* DataTable;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;
	

	
	// ================================
	// =        FUNCTIONS            =
	// ================================


	
	void AddItem(FName RowName, int32 Quantity);
	virtual UStaticMesh* GetItemMesh_Implementation (FName RowName) override;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
};
