#pragma once

UENUM()
enum class EInventoryUpdateType : uint8
{
	Create		UMETA(DisplayName = "Add"),
	Insert  UMETA(DisplayName = "Insert"),
	Remove  UMETA(DisplayName = "Remove"),
	Swap    UMETA(DisplayName = "Swap"),
	Existing    UMETA(DisplayName = "Existing"),
};

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"


class UInventoryHUD;
class AInv_MasterItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_InteractionInterface
{
	GENERATED_BODY()

	
protected:
	
	virtual void BeginPlay() override;

	UInventoryComponent(); //Constructor
	
public:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", DisplayName = "HUD Widget")
	TSubclassOf<UInventoryHUD> HUDWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* DataTable = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;
	
	UPROPERTY()
	TSubclassOf<AInv_MasterItem> ItemClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryHUD> InventoryHUD;
		
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void AddItem(FName RowName, int32 Quantity);
	
	void SplitItem(int32 IndexToSplit, int32 QuantityToSplit);

	void RemoveItem(int32 Index);
	
	void SwapItem(int32 SourceIndex, int32 DestinationIndex);

	void DropItemQuantity(int32 SlotIndex, int32 QuantityToSubtract);
	
private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================
	
	TWeakObjectPtr<APlayerController> OwningController;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void CreateHUDWidget();

	void CreateDefaults();
	
	void StackOnAdd(const FItemData* Item);

	void StackOnSwap (int32 DraggedIndex, int32 DestinationIndex);
	
	void UpdateOnSwap (const TArray<int32>& IndexesToUpdate);

	void UpdateOnAdd(const TArray<int32>& IndexesToUpdate);

	void UpdateOnSplit(const TArray<int32>& IndexesToUpdate);

	void UpdateOnRemove(const TArray<int32>& IndexesToUpdate);

	// ================================
	// =        TEMPLATES            =
	// =================================

	template<typename... Indexes>
	void UpdateInventorySlot(EInventoryUpdateType UpdateType, Indexes... ModifiedIndexes);
};
