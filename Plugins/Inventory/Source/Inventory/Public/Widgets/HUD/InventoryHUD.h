#pragma once

UENUM()
enum class EHUDUpdates : uint8
{
	Create	UMETA(DisplayName = "Create"),
	Insert  UMETA(DisplayName = "Insert"),
	Remove  UMETA(DisplayName = "Remove"),
	Existing   UMETA(DisplayName = "Existing")
};



#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryHUD.generated.h"

class UWrapBox;
class UInventoryComponent;
class UInv_InteractWidget;
class UInv_ItemSlot;
class UInv_OnDragSlot;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInventoryHUD : public UUserWidget, public IInv_InteractionInterface
{
	GENERATED_BODY()
	
private:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_InteractWidget> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_OnDragSlot> DragSlotClass;
	
	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;
			
	// ================================
	// =       FUNCTIONS           =
	// ================================
    	
	void CreateSlot(TArray<int32> IndexesToUpdate);
    
	void InsertSlot(TArray<int32> IndexesToUpdate);
    	
	void RemoveSlot(TArray<int32> IndexesToUpdate);

	void UpdateExistingSlot(TArray<int32> IndexesToUpdate);
	
public:
	
	// ================================
	// =       PROPERTIES          =
	// ================================

	UPROPERTY()
	UInventoryComponent* PlayerInventory;

	UPROPERTY()
	TArray<UInv_ItemSlot*> InventorySlots;
	
	UPROPERTY()
	bool bInventoryOpen;

	// ================================
	// =       FUNCTIONS           =
	// ================================
	
	bool ToggleHUD();
	
	void UpdateIndexes();

	template<typename... Indexes>
	void UpdateSlots(EHUDUpdates UpdateType, Indexes... ModifiedIndexes)
	{
		TArray<int32> IndexesToUpdate = { ModifiedIndexes... };

		if (!PlayerInventory) return;
		
		UpdateIndexes();
		
		switch (UpdateType)
		{
		case EHUDUpdates::Create:
			CreateSlot(IndexesToUpdate);
			break;
        
		case EHUDUpdates::Remove:
			RemoveSlot(IndexesToUpdate);
			break;
        
		case EHUDUpdates::Insert:
			InsertSlot(IndexesToUpdate);
			break;

		case EHUDUpdates::Existing:
			UpdateExistingSlot(IndexesToUpdate);
			break;
		}
		UpdateIndexes();
	}
	
protected:
	
	virtual void NativeConstruct() override;
	
	
};
