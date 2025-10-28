#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UUIManagerSubsystem;

UCLASS()
class ACTIONGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
private:

	// ================================
	// =        PROPERTIES           =
	// ================================

	UPROPERTY()
	UUIManagerSubsystem* UIManagerSubsystem;
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext = nullptr;
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MouseMappingContext = nullptr;

	UPROPERTY(EditAnywhere)
	UInputAction* OpenInventory;

	// ================================
	// =        FUNCTIONS            =
	// ================================

	void GetUIManagerSubsystem();
	
	void InitializePlayerInventory() const;

	void InitializeHUD() const;

	void AddMappingContext() const;

	void OnInventoryInputPressed();
	
};
