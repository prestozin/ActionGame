#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class ACTIONGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	
private:

	// ================================
	// =        PROPERTIES           =
	// ================================
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* DefaultMappingContext = nullptr;

	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MouseMappingContext = nullptr;

	// ================================
	// =        FUNCTIONS            =
	// ================================
	
	void InitializePlayerInventory() const;

	void InitializeMainHud() const;

	void ConfigureInputSystem() const;
	
};
