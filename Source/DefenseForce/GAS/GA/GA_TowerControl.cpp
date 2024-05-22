// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_TowerControl.h"
#include "DFLog.h"

void UGA_TowerControl::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	DF_NETGASLOG(LogDF, Log, TEXT("Start"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
