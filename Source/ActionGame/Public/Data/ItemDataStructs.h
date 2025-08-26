#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

//ENUMS

UENUM()
enum class EItemRarity : uint8
{
	Common UMETA( DisplayName = "Common" ),
	Rare UMETA( DisplayName = "Rare" ),
	Epic UMETA( DisplayName = "Epic" ),
	Legendary UMETA( DisplayName = "Legendary" )
};

UENUM()
enum class EItemType : uint8
{
	Weapon UMETA( DisplayName = "Weapon" ),
	Consumables UMETA( DisplayName = "Consumables" ),
	Talismans UMETA( DisplayName = "Talismans" )
};


// STRUCTS

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;
};

USTRUCT()
struct  FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 Quantity;
    
	UPROPERTY(EditAnywhere)
	int32 MaxQuantity;

	UPROPERTY(EditAnywhere)
	bool IsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	USoundCue* Sound;
	
	
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemTextData;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemRarity ItemRarity;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumericData;
	



	
	UPROPERTY(EditAnywhere, Category = "Item Data")
    	FItemAssetData ItemAssetData;
	
};