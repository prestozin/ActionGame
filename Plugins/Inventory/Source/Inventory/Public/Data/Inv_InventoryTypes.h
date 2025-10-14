#pragma once
UENUM(BlueprintType)
enum class EInventoryUpdateType : uint8
{
	Create		UMETA(DisplayName = "Add"),
	Insert		UMETA(DisplayName = "Insert"),
	Remove		UMETA(DisplayName = "Remove"),
	Update		UMETA(DisplayName = "Update"),
};

class Inv_InventoryTypes

{
public:

};
