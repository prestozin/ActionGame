// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inv_MasterItem.h"
#include "Components/InventoryComponent.h"


AInv_MasterItem::AInv_MasterItem()
{
	//add tag
	Tags.Add(FName("Item"));

	//create and configure mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMesh->SetCollisionProfileName(TEXT("Item"));
	ItemMesh->SetGenerateOverlapEvents(true);
	RootComponent = ItemMesh;
}

void AInv_MasterItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemMesh();
	
	
}

#pragma region GetItemData

void AInv_MasterItem::GetItemData_Implementation(FName& OutItemName, int32& OutQuantity)
{
	OutItemName = Name;
	OutQuantity = Quantity;
}

void AInv_MasterItem::SetItemMesh() const
{
	//cria um actor component chamado item component, procura ele no mundo e pega dele o component do inventario
	UActorComponent* ItemComponent = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UInventoryComponent>();

	//verifica se o item component é valido e tem a interface aplicada
	if (ItemComponent && ItemComponent->GetClass() == UInventoryComponent::StaticClass())
	{
		//seta a mesh do item para ser a mesh recebida pela interface, passando seu nome para ser procurado na data table
		UStaticMesh* DataTableMesh = IInv_InteractionInterface::Execute_GetItemMesh(ItemComponent, Name);
		ItemMesh->SetStaticMesh(DataTableMesh);
	}
}

#pragma endregion

