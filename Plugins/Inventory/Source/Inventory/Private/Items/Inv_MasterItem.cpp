// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inv_MasterItem.h"



AInv_MasterItem::AInv_MasterItem()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	Tags.Add(FName("Item"));
}


void AInv_MasterItem::GetItemData_Implementation(FName& OutItemName, int32& OutQuantity)
{
	OutItemName = Name;
	OutQuantity = Quantity;
}

void AInv_MasterItem::BeginPlay()
{
	Super::BeginPlay();

	
	
}



