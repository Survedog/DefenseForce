// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_BuildStructure.h"
#include "DFLog.h"

void UGA_BuildStructure::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Don't call original super function.
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
