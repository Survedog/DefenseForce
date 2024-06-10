// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_TowerControl.h"
#include "GAS/AT/DFAT_WaitTargetData_ReusableTA.h"
#include "Subsystem/TargetingInstanceSubsystem.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "AbilitySystemComponent.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "Structure/DFTowerBase.h"
#include "Physics/DFCollision.h"
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

	IPlayerTowerControlInterface* TowerControlInterface = CastChecked<IPlayerTowerControlInterface>(ActorInfo->AvatarActor);
	ADFTowerBase* TowerToControl = Cast<ADFTowerBase>(TowerControlInterface->GetCurrentStructureUnderCursor());
	if (!TowerToControl)
	{
		K2_CancelAbility();
	}

	if (UTargetingInstanceSubsystem* TargetingInstanceSubsystem = ULocalPlayer::GetSubsystem<UTargetingInstanceSubsystem>(ActorInfo->PlayerController->GetLocalPlayer()))
	{
		if (DFTargetActor = TargetingInstanceSubsystem->GetTargetActorFromClass<ADFGATA_Trace>(*TowerToControl->GetTargetActorClass()))
		{
			DFTargetActor->SetOwner(TowerToControl);
			DFTargetActor->SetTraceChannel(CCHANNEL_PlayerAim);
			DFTargetActor->bIgnoreBlockingHits = false;
			DFTargetActor->bTraceStartsFromPlayerCamera = true;
			DFTargetActor->bTraceTowardMouseAimLocation = true;
		}
	}

	if (DFTargetActor)
	{		
		DFTargetActor->ReticleClass = TowerToControl->GetReticleClass();
		GetAbilitySystemComponentFromActorInfo()->SpawnedTargetActors.Push(DFTargetActor);

		// TODO: Reuse AbilityTask
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
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (IsLocallyControlled())
	{
		ASC->SpawnedTargetActors.Remove(DFTargetActor);
		DFTargetActor->SetOwner(nullptr);
		DFTargetActor = nullptr;
	}

	FGameplayTagContainer AttackTagContainer(GASTAG_Structure_Action_Attack);
	ASC->CancelAbilities(&AttackTagContainer);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_TowerControl::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));	
	IPlayerTowerControlInterface* TowerControlInterface = CastChecked<IPlayerTowerControlInterface>(ActorInfo->AvatarActor);
	ADFTowerBase* ControlledTower = TowerControlInterface->GetCurrentControlledTower();
	if (ControlledTower && ControlledTower->ShouldConfirmTargetOnInputPressed())
	{
		DFTargetActor->ConfirmTargetingAndContinue();
	}
}

void UGA_TowerControl::OnTargetDataReadyCallback_Implementation(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	FGameplayEventData Payload;
	Payload.TargetData = TargetDataHandle;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (!ASC->HasMatchingGameplayTag(GASTAG_Structure_Action_Attack))
	{
		SendGameplayEvent(GASTAG_Structure_Action_Attack, Payload);
	}
	else
	{
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(EDFAbilityInputID::Attack));
		if (Spec)
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
	}
}
