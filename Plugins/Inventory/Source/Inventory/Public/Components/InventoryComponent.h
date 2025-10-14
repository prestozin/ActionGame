#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_InventoryTypes.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "Interfaces/Inv_InventoryListener.h"
#include "Interfaces/Inv_InventorySetup.h"
#include "Interfaces/Inv_InventoryActions.h"
#include "InventoryComponent.generated.h"

class AInv_MasterItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_InventoryListener, public IInv_InventorySetup, public IInv_InventoryActions, public IInv_InteractionInterface
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	UInventoryComponent(); //Constructor
	
private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================

	UPROPERTY()
	TArray<TScriptInterface<IInv_InventoryListener>> InventoryListeners;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;
	
	TWeakObjectPtr<APlayerController> OwningController;
	
	UPROPERTY()
	TSubclassOf<AInv_MasterItem> ItemClass;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	void BroadcastInventoryChanges(EInventoryUpdateType UpdateType, const TArray<int32>& ModifiedIndexes);
	
	virtual void RegisterListener_Implementation(UObject* ObjectListener) override;
			
	void SplitItem(int32 IndexToSplit, int32 QuantityToSplit);

	void RemoveItem(int32 Index);
	
	void SwapItem(int32 SourceIndex, int32 DestinationIndex);

	void DropItemQuantity(int32 SlotIndex, int32 QuantityToSubtract);
	
	void StackOnAdd(const FItemData* Item);

	void StackOnSwap (int32 DraggedIndex, int32 DestinationIndex);
		
public:
	
	void AddItem(FName RowName, int32 Quantity);
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* DataTable = nullptr;
	
	// ================================
	// =     INTERFACE FUNCTIONS      =
	// =================================
	
	virtual int32 GetSlotQuantity_Implementation (int32 Index) override;

	virtual int32 GetInventorySize_Implementation() override;

	virtual UTexture2D* GetSlotIcon_Implementation(int32 Index) override;

	virtual FText GetSlotName_Implementation(int32 Index) override;

	virtual FText GetSlotDescription_Implementation(int32 Index) override;

	virtual EItemType GetSlotType_Implementation(int32 Index) override;

	virtual EItemRarity GetSlotRarity_Implementation(int32 Index) override;

	virtual void ISplitItem_Implementation(int32 IndexToSplit, int32 QuantityToSplit) override;

	virtual void IRemoveItem_Implementation(int32 Index) override;
	
	virtual void ISwapItem_Implementation(int32 SourceIndex, int32 DestinationIndex) override;

	virtual void IDropItemQuantity_Implementation(int32 SlotIndex, int32 QuantityToSubtract) override;

	template <typename InterfaceType>
	static TScriptInterface<InterfaceType> MakeInterface(UObject* Object)
	{
		TScriptInterface<InterfaceType> Interface;
		if (!Object) return Interface;

		if (Object->GetClass()->ImplementsInterface(InterfaceType::UClassType::StaticClass()))
		{
			Interface.SetObject(Object);
			Interface.SetInterface(Cast<InterfaceType>(Object));
		}
		return Interface;
	}
};
