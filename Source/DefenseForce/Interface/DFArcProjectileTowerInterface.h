// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DFArcProjectileTowerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDFArcProjectileTowerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEFENSEFORCE_API IDFArcProjectileTowerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual FVector GetProjectileSpawnLocation() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual float GetProjectileLaunchSpeed() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual float GetProjectileLaunchPitch() const = 0;
};
