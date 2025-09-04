// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Inv_PlayerCharacter.generated.h"

class UInventoryComponent;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class INVENTORY_API AInv_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AInv_PlayerCharacter();
	
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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction*  JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction*  InteractAction;
	
	// ================================
	// =        FUNCTIONS           =
	// ================================
	
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact();

private:
	
	// ================================
	// =        PROPERTIES            =
	// ================================

	UPROPERTY()
	AActor* NearActor = nullptr;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractSphere;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;
	
	// ================================
	// =        FUNCTIONS            =
	// ================================

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
