#pragma once

UENUM()
enum class EInventoryUpdateType : uint8
{
	Add UMETA(DisplayName = "Add"),
	Insert  UMETA(DisplayName = "Insert"),
	Remove  UMETA(DisplayName = "Remove"),
	Swap    UMETA(DisplayName = "Swap")
};

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"

class UUserWidget;
class UInventoryHUD;
class UInv_MasterItem;
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
	UDataTable* DataTable = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryHUD> HUDWidgetClass;

	UPROPERTY()
	TSubclassOf<AInv_MasterItem> ItemClass;
	
	UPROPERTY()
	TObjectPtr<UInventoryHUD> InventoryHUD;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_InteractWidget> InteractWidgetClass;

	UPROPERTY()
	UInv_InteractWidget* InteractWidget = nullptr;

	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void AddItem(FName RowName, int32 Quantity);
	
	void SplitStack (int32 Index, int32 QuantityToSplit);

	void RemoveItem(int32 Index);
	
	template<typename... Indexes>
	void UpdateInventorySlot(EInventoryUpdateType UpdateType, Indexes... ModifiedIndexes);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SwapItemSlot(int32 SourceIndex, int32 DestinationIndex);

	virtual UObject* GetItemMesh_Implementation (FName RowName) override;
	
	const void SpawnItem(const FItemData& Item);
	
private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================
	
	
	TWeakObjectPtr<APlayerController> OwningController;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	void StackOnAdd(const FItemData* Item);

	void StackOnSwap (int32 DraggedIndex, int32 DestinationIndex);

	void CreateHUDWidget();

	void CreateDefaults();

	void UpdateOnSwap (const TArray<int32>& IndexesToUpdate);

	void UpdateOnAdd(const TArray<int32>& IndexesToUpdate);

	void UpdateOnInsert(const TArray<int32>& IndexesToUpdate);

	void UpdateOnRemove(const TArray<int32>& IndexesToUpdate);
};
