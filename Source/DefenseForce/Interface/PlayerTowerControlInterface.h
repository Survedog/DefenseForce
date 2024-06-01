// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerTowerControlInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerTowerControlInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEFENSEFORCE_API IPlayerTowerControlInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartTowerControl(class ADFTowerBase* NewTower);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndTowerControl();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	class ADFTowerBase* GetCurrentControlledTower();
};
