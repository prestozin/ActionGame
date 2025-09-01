// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInventoryComponent;
class USphereComponent;
class UInputMappingContext;
class UInputAction;

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

	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction*  LookAction;

	// ================================
	// =        FUNCTIONS           =
	// ================================
	
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();

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
