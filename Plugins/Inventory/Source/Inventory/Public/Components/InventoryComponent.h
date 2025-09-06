

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "InventoryComponent.generated.h"

class UUserWidget;
class UInventoryHUD;

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


	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void AddItem(FName RowName, int32 Quantity);
	virtual UStaticMesh* GetItemMesh_Implementation (FName RowName) override;

private:
	
	// ================================
	// =        PROPERTIES          =
	// =================================

	
	//pega o player controller do owner do component e armazena em uma variavel
	TWeakObjectPtr<APlayerController> OwningController;


	
	// ================================
	// =        FUNCTIONS            =
	// =================================

	void CreateHUDWidget();
};
