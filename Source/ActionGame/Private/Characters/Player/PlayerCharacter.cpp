// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/InteractionInterface.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSphereOverlap);
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// se other actor for valido, tiver a tag e tiver a interface, passe para proxima etapa
	if (OtherActor && OtherActor->ActorHasTag("Item") && OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		FName ItemName;
		int32 Quantity;

		// pegue a interface do other actor, e chame a função preenchida, defina de quem é a função (other actor) e preencha os inputs
		OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass());
		IInteractionInterface::Execute_GetItemData(OtherActor, ItemName, Quantity);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *ItemName.ToString());

		if (PlayerInventory)
		{
			PlayerInventory->AddItem(ItemName, Quantity);

			OtherActor->Destroy();
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item e: %s não encontrado"), *ItemName.ToString());
		}
	}
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




