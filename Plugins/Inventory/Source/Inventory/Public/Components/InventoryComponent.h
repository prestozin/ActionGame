#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_InventoryTypes.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_IInteract.h"
#include "Interfaces/Inv_IInventoryObserver.h"
#include "Interfaces/Inv_IInventoryReader.h"
#include "Interfaces/Inv_IInventoryHandler.h"
#include "InventoryComponent.generated.h"

class AInv_MasterItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UInventoryComponent : public UActorComponent, public IInv_IInventoryObserver, public IInv_IInventoryReader, public IInv_IInventoryHandler, public IInv_IInteract
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
	TArray<TScriptInterface<IInv_IInventoryObserver>> InventoryListeners;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FItemData> Inventory;
	
	UPROPERTY()
	TSubclassOf<AInv_MasterItem> ItemClass;
	
	// ================================
	// =        FUNCTIONS            =
	// =================================
	
	void BroadcastInventoryChanges(EInventoryUpdateType UpdateType, const TArray<int32>& ModifiedIndexes);
	
	virtual void RegisterObserver_Implementation(UObject* ObjectListener) override;
			
	void SplitItem(int32 IndexToSplit, int32 QuantityToSplit);
	
	void SwapItem(int32 SourceIndex, int32 DestinationIndex);

	void DropItem(int32 SlotIndex, int32 QuantityToDrop = -1);
	
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
	
	virtual void ISwapItem_Implementation(int32 SourceIndex, int32 DestinationIndex) override;

	virtual void IDropItem_Implementation(int32 SlotIndex, int32 QuantityToDrop) override;

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
