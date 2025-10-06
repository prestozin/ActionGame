// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inv_MasterItem.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Kismet/GameplayStatics.h"

AInv_MasterItem::AInv_MasterItem()
{
	//add tag
	Tags.Add(FName("Item"));

	//create root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	//create and configure static mesh
	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemStaticMesh->SetCollisionProfileName(TEXT("Item"));
	ItemStaticMesh->SetGenerateOverlapEvents(true);
	ItemStaticMesh->SetupAttachment(Root);

	//create and configure skeletal mesh
	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemSkeletalMesh->SetCollisionProfileName(TEXT("Item"));
	ItemSkeletalMesh->SetGenerateOverlapEvents(true);
	ItemSkeletalMesh->SetupAttachment(Root);
}

void AInv_MasterItem::BeginPlay()
{
	Super::BeginPlay();
	SetItemMesh();
}

#pragma region GetItemData

void AInv_MasterItem::GetItemData_Implementation(FName& OutItemName, int32& OutQuantity)
{
	OutItemName = ID;
	OutQuantity = Quantity;
}

void AInv_MasterItem::SpawnItem(UWorld* World, UDataTable* OtherDataTable, FName ItemID, int32 ItemQuantity, const FVector SpawnLocation, AActor* Owner)
{
	if (!World || !OtherDataTable || ItemID.IsNone() || ItemQuantity <= 0) return;
		
	FItemData* ItemToSpawn = OtherDataTable->FindRow<FItemData>(ItemID, TEXT("Item"));
	
	if (!ItemToSpawn) return;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParameters.Owner = Owner;
	
	AInv_MasterItem* SpawnedItem=  World->SpawnActorDeferred<AInv_MasterItem>(StaticClass(), FTransform(FRotator::ZeroRotator, SpawnLocation), Owner);
	
	if (!SpawnedItem) return;
	
	SpawnedItem->ID = ItemID;
	SpawnedItem->Quantity = ItemQuantity;
	SpawnedItem->DataTable = OtherDataTable;

	UGameplayStatics::FinishSpawningActor(SpawnedItem, FTransform(FRotator::ZeroRotator, SpawnLocation));
}

void AInv_MasterItem::SetItemMesh() const
{
	if (!DataTable) return;

	FItemData* Item = DataTable->FindRow<FItemData>(ID, TEXT("SetItemMesh"));
	
	if (UStaticMesh* StaticMesh = Item->ItemAssetData.StaticMesh.LoadSynchronous())
	{
		if (IsValid(ItemStaticMesh))
		{
			ItemStaticMesh->SetStaticMesh(StaticMesh);
			ItemSkeletalMesh->SetSkeletalMesh(nullptr);
		}
	}
	else if (USkeletalMesh* SkeletalMesh = Item->ItemAssetData.SkeletalMesh.LoadSynchronous())
	{
		if (IsValid(ItemSkeletalMesh))
		{
			ItemSkeletalMesh->SetSkeletalMesh(SkeletalMesh);
			ItemStaticMesh->SetStaticMesh(nullptr);
		}
	}
}

#pragma endregion

