#pragma once

UENUM()
enum class EInventoryUpdate : uint8
{
	Create		UMETA(DisplayName = "Add"),
	Insert  UMETA(DisplayName = "Insert"),
	Remove  UMETA(DisplayName = "Remove"),
	Update    UMETA(DisplayName = "Update"),
};

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"

#pragma region Delegates

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, EInventoryUpdate /*update type*/, const TArray<int32>& /*index*/)

#pragma endregion

class AInv_MasterItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_InteractionInterface
{
	GENERATED_BODY()

	
protected:
	
	virtual void BeginPlay() override;

	UInventoryComponent(); //Constructor
	
private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================
	
	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;
	
	TWeakObjectPtr<APlayerController> OwningController;
	
	UPROPERTY()
	TSubclassOf<AInv_MasterItem> ItemClass;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void CreateDefaults();
	
	void StackOnAdd(const FItemData* Item);

	void StackOnSwap (int32 DraggedIndex, int32 DestinationIndex);
	
	// ================================
	// =        TEMPLATES            =
	// =================================

	template<typename... Indexes>
	void UpdateInventory(EInventoryUpdate UpdateType, Indexes... ModifiedIndexes);
		
public:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* DataTable = nullptr;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void AddItem(FName RowName, int32 Quantity);
	
	void SplitItem(int32 IndexToSplit, int32 QuantityToSplit);

	void RemoveItem(int32 Index);
	
	void SwapItem(int32 SourceIndex, int32 DestinationIndex);

	void DropItemQuantity(int32 SlotIndex, int32 QuantityToSubtract);

	int32 GetInventorySize() const{ return Inventory.Num(); }

	bool IsValidSlot(int32 Index) const { return Inventory.IsValidIndex(Index); }
	
	const FItemData* GetItemAt(int32 Index) const{ return Inventory.IsValidIndex(Index) ? &Inventory[Index] : nullptr;}

	// ================================
	// =        DELEGATES           =
	// =================================

	FOnInventoryChanged OnInventoryChanged;

};
