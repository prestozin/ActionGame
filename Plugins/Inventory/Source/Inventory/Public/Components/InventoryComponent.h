

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"

class UUserWidget;
class UInventoryHUD;
class UInv_InteractWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_InteractionInterface
{
	GENERATED_BODY()

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
public:
	
	UInventoryComponent(); //Constructor

	
	// ================================
	// =        PROPERTIES            =
	// ================================

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* DataTable;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryHUD> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryHUD> InventoryHUD;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_InteractWidget> InteractWidgetClass;

	UPROPERTY()
	UInv_InteractWidget* InteractWidget;

	bool bShouldStackedItem;

	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void AddItem(FName RowName, int32 Quantity);
	
	void StackItem(FItemData* Item);
	
	virtual UStaticMesh* GetItemMesh_Implementation (FName RowName) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItemSlot(int32 SourceIndex, int32 DestinationIndex);


private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================

	
	//get the owner player controller
	TWeakObjectPtr<APlayerController> OwningController;

	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void CreateHUDWidget();

	void CreateDefaults();
	
};
