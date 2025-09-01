

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemDataStructs.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UInventoryComponent : public UActorComponent
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

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
};
