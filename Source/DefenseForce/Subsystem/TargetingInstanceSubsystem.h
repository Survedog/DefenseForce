// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "TargetingInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UTargetingInstanceSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	class AGameplayAbilityTargetActor* GetTargetActorFromClass(TSubclassOf<class AGameplayAbilityTargetActor> InTargetActorClass);
	class AGameplayAbilityTargetActor* GetTargetActorFromClass(TSubclassOf<class AGameplayAbilityTargetActor> InTargetActorClass) const;

protected:
	virtual void Deinitialize() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	TMap<TSubclassOf<class AGameplayAbilityTargetActor>, TObjectPtr<class AGameplayAbilityTargetActor>> TAClassInstanceMap;
};
