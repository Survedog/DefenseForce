// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_TowerControl.h"
#include "GAS/AT/DFAT_WaitTargetData_ReusableTA.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AbilitySystemComponent.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "Structure/DFTowerBase.h"
#include "GAS/DFGameplayTags.h"
#include "DefenseForce.h"
#include "DFLog.h"

void UGA_TowerControl::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));

	if (!IsLocallyControlled())
	{
		return;
	}
	
	ADFTowerBase* TowerToControl = CastChecked<ADFTowerBase>(ActorInfo->AvatarActor);
	DFTargetActor = TowerToControl->GetAttackTargetActor();
	if (DFTargetActor)
	{		
		TowerToControl->GetAbilitySystemComponent()->SpawnedTargetActors.Push(DFTargetActor);
		UDFAT_WaitTargetData_ReusableTA* WaitTargetDataTask = UDFAT_WaitTargetData_ReusableTA::WaitTargetDataUsingReusableTA(this, FName("WaitAttackTargetData"), EGameplayTargetingConfirmation::Type::CustomMulti, DFTargetActor);
		if (WaitTargetDataTask)
		{
			WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_TowerControl::OnTargetDataReadyCallback);
			WaitTargetDataTask->ReadyForActivation();
		}
	}
	else
	{
		DF_NETGASLOG(LogDFGAS, Error, TEXT("Target actor is nullptr."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGA_TowerControl::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start. EndState: %s"), bWasCancelled ? TEXT("Cancelled") : TEXT("Confirmed"));
	UAbilitySystemComponent* TowerASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (IsLocallyControlled())
	{
		TowerASC->SpawnedTargetActors.Remove(DFTargetActor);
		DFTargetActor->SetOwner(nullptr);
		DFTargetActor = nullptr;
	}

	FGameplayTagContainer AttackTagContainer(GASTAG_Structure_Action_Attack);
	TowerASC->CancelAbilities(&AttackTagContainer);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_TowerControl::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));	
	ADFTowerBase* ControlledTower = CastChecked<ADFTowerBase>(ActorInfo->AvatarActor);
	if (ControlledTower && ControlledTower->ShouldConfirmTargetOnInputPressed())
	{
		DFTargetActor->ConfirmTargetingAndContinue();
	}
}

void UGA_TowerControl::OnTargetDataReadyCallback_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	UAbilitySystemComponent* TowerASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (!TowerASC->HasMatchingGameplayTag(GASTAG_Structure_Action_Attack))
	{
		FGameplayEventData Payload;
		Payload.TargetData = TargetDataHandle;
		SendGameplayEvent(GASTAG_Structure_Action_Attack, Payload);
	}
}
