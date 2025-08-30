// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MasterItem.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"


AMasterItem::AMasterItem()
{
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(Sphere);
	SetRootComponent(Sphere);
}



void AMasterItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GetItem(OtherActor);
}

void AMasterItem::GetItem(AActor* ItemActor)
{
	if (ItemActor->ActorHasTag("Player"))
	{
		UInventoryComponent* Inventory =  ItemActor->FindComponentByClass<UInventoryComponent>();
		if (Inventory)
		{
			Inventory->AddItem(ItemName);
		}

		Destroy();
		
		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *ItemName.ToString());
	}
}


void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnSphereOverlap);
}

void AMasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

