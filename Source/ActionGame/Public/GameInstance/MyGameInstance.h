// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class UMainHud;

UCLASS()
class ACTIONGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHud> HUDClass;
};
