// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MasterItem.h"



AMasterItem::AMasterItem()
{
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
}


void AMasterItem::GetItemData_Implementation(FName& OutItemName, int32& OutQuantity)
{
	OutItemName = Name;
	OutQuantity = Quantity;
}

void AMasterItem::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Item"));
	
}



