// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInventoryComponent;
class USphereComponent;
UCLASS()
class ACTIONGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	
	UPROPERTY(EditAnywhere)
	UInventoryComponent* PlayerInventory;
	
	
protected:
	
	virtual void BeginPlay() override;

private:
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractSphere;


	// ================================
	// =        FUNCTIONS            =
	// ================================

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
