// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_BuildStructure.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "DFLog.h"

//void UGA_BuildStructure::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
//{
//	// Don't call original super function.
//	bool bReplicateEndAbility = true;
//	bool bWasCancelled = false;
//	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

void UGA_BuildStructure::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass = ADFGATA_Trace::StaticClass();
	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(this, FName("WaitBuildTargetData"), EGameplayTargetingConfirmation::Type::UserConfirmed, TargetActorClass);
	if (WaitTargetDataTask)
	{
		AGameplayAbilityTargetActor* TargetActor = nullptr;
		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_BuildStructure::OnTargetDataReadyCallback);
		WaitTargetDataTask->Cancelled.AddDynamic(this, &UGA_BuildStructure::OnTargetDataCancelledCallback);
		if (WaitTargetDataTask->BeginSpawningActor(this, TargetActorClass, TargetActor))
		{
			WaitTargetDataTask->FinishSpawningActor(this, TargetActor);
		}
	}
}

void UGA_BuildStructure::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}

void UGA_BuildStructure::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Don't call original super function.
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}

void UGA_BuildStructure::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}

void UGA_BuildStructure::OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}
