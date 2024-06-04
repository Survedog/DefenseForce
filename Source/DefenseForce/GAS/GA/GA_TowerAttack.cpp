// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_TowerAttack.h"
#include "GAS/AT/DFAT_WaitTargetData_ReusableTA.h"
#include "Subsystem/TargetingInstanceSubsystem.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "AbilitySystemComponent.h"
#include "DFLog.h"

UGA_TowerAttack::UGA_TowerAttack()
{

}

void UGA_TowerAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));

	if (!IsLocallyControlled())
	{
		return;
	}

	if (!DFTargetActor)
	{
		if (UTargetingInstanceSubsystem* TargetingInstanceSubsystem = ULocalPlayer::GetSubsystem<UTargetingInstanceSubsystem>(ActorInfo->PlayerController->GetLocalPlayer()))
		{
			if (DFTargetActor = TargetingInstanceSubsystem->GetTargetActorFromClass<ADFGATA_Trace>(*TargetActorClass))
			{
				DFTargetActor->TraceProfile = FCollisionProfileName(TEXT("BlockOnlyWorld"));
				DFTargetActor->bIgnoreBlockingHits = false;
				DFTargetActor->bTraceStartsFromPlayerCamera = true;
				DFTargetActor->bTraceTowardMouseAimLocation = true;
				DFTargetActor->ReticleClass = ReticleClass;
			}
		}
	}

	if (DFTargetActor)
	{
		GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Push(DFTargetActor);

		// TODO: Reuse AbilityTask
		UDFAT_WaitTargetData_ReusableTA* WaitTargetDataTask = UDFAT_WaitTargetData_ReusableTA::WaitTargetDataUsingReusableTA(this, FName("WaitAttackTargetData"), EGameplayTargetingConfirmation::Type::UserConfirmed, DFTargetActor);
		if (WaitTargetDataTask)
		{
			WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_TowerAttack::OnTargetDataReadyCallback);
			WaitTargetDataTask->Cancelled.AddDynamic(this, &UGA_TowerAttack::OnTargetDataCancelledCallback);
			WaitTargetDataTask->ReadyForActivation();
		}
	}
	else
	{
		DF_NETGASLOG(LogDFGAS, Error, TEXT("Target actor is nullptr."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGA_TowerAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start. EndState: %s"), bWasCancelled ? TEXT("Cancelled") : TEXT("Confirmed"));
	GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Remove(DFTargetActor);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_TowerAttack::OnTargetDataReadyCallback_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}

void UGA_TowerAttack::OnTargetDataCancelledCallback_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
}
