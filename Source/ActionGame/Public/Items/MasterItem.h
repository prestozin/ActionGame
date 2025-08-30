// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/ItemDataStructs.h"
#include "MasterItem.generated.h"


UCLASS()
class ACTIONGAME_API AMasterItem : public AActor
{
	GENERATED_BODY()

public:
 AMasterItem();
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	
	
	UPROPERTY (EditAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;
    
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ItemName;

	;


	
	// ================================
	// =        FUNCTIONS             =
	// ================================



	
	
	
protected:

	
	

	virtual void BeginPlay() override;
private:

	// ================================
	// =        PROPERTIES            =
	// ================================


	UPROPERTY(EditAnywhere)
	class USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;


	// ================================
	// =        FUNCTIONS             =
	// ================================

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void GetItem(AActor* ItemActor);
	
public:
	
	virtual void Tick(float DeltaTime) override;
};
