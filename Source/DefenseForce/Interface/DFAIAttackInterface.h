// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "DFAIAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UDFAIAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEFENSEFORCE_API IDFAIAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EBTNodeResult::Type Attack(AActor* InAttackTarget);

	virtual EBTNodeResult::Type Attack_Implementation(AActor* InAttackTarget) = 0;
};
