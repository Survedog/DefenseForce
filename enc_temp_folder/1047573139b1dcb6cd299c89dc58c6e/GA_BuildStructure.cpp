// Copyright 2024, Survedog. All rights reserved.


#include "GAS/GA/GA_BuildStructure.h"
#include "GAS/AT/DFAT_WaitTargetData_ReusableTA.h"
#include "GAS/TA/DFGATA_ActorPlacement.h"
#include "GAS/TA/Reticle/GAWorldReticle_ActorVisualization.h"
#include "Structure/DFStructureBase.h"
#include "Interface/PlayerBuildModeInterface.h"
#include "Net/UnrealNetwork.h"
#include "DFLog.h"

UGA_BuildStructure::UGA_BuildStructure() : TargetStructureClass(nullptr), BuiltStructure(nullptr), DFActorPlacementTA(nullptr)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	static ConstructorHelpers::FClassFinder<ADFGATA_ActorPlacement> TargetActorClassRef(TEXT("/Game/DefenseForce/Blueprint/GAS/TA/BP_DFGATA_ActorPlacement.BP_DFGATA_ActorPlacement_C"));
	if (TargetActorClassRef.Class)
	{
		ActorPlacementTAClass = TargetActorClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AGAWorldReticle_ActorVisualization> ReticleClassRef(TEXT("/Game/DefenseForce/Blueprint/GAS/TA/ReticleActor/BP_GAWorldReticle_ActorVisualization.BP_GAWorldReticle_ActorVisualization_C"));
	if (ReticleClassRef.Class)
	{
		ActorVisualReticleClass = ReticleClassRef.Class;
	}	
}

void UGA_BuildStructure::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGA_BuildStructure, BuiltStructure, COND_OwnerOnly, REPNOTIFY_Always);
}

void UGA_BuildStructure::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	
	BuiltStructure = nullptr;

	if (IsLocallyControlled())
	{
		if (!DFActorPlacementTA)
		{
			DF_NETGASLOG(LogDFGAS, Log, TEXT("Spawning target actor."));
			DFActorPlacementTA = GetWorld()->SpawnActor<ADFGATA_ActorPlacement>(ActorPlacementTAClass, FTransform::Identity);
			DFActorPlacementTA->TraceProfile = FCollisionProfileName(TEXT("BlockOnlyWorld"));
			DFActorPlacementTA->bIgnoreBlockingHits = false;
			DFActorPlacementTA->bTraceStartsFromPlayerCamera = true;
			DFActorPlacementTA->bTraceTowardMouseAimLocation = true;
			DFActorPlacementTA->ReticleClass = ActorVisualReticleClass;
		}

		TargetStructureClass = TriggerEventData->OptionalObject->GetClass();
		if (TargetStructureClass != DFActorPlacementTA->GetPlacedActorClass())
		{
			DFActorPlacementTA->SetPlacedActorClass(TargetStructureClass);
		}

		if (DFActorPlacementTA)
		{
			// TODO: Reuse AbilityTask
			UDFAT_WaitTargetData_ReusableTA* WaitTargetDataTask = UDFAT_WaitTargetData_ReusableTA::WaitTargetDataUsingReusableTA(this, FName("WaitBuildTargetData"), EGameplayTargetingConfirmation::Type::UserConfirmed, DFActorPlacementTA);
			if (WaitTargetDataTask)
			{
				WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_BuildStructure::OnTargetDataReadyCallback);
				WaitTargetDataTask->Cancelled.AddDynamic(this, &UGA_BuildStructure::OnTargetDataCancelledCallback);
				WaitTargetDataTask->ReadyForActivation();
			}
		}
		else
		{
			DF_NETGASLOG(LogDFGAS, Error, TEXT("Target actor is nullptr."));
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		}
	}
}

void UGA_BuildStructure::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	if (APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor))
	{
		if (IPlayerBuildModeInterface* BuildModeInterface = Cast<IPlayerBuildModeInterface>(AvatarPawn->GetController()))
		{
			BuildModeInterface->ExitBuildMode(BuiltStructure.Get());
		}
	}
	TargetStructureClass = nullptr;
	BuiltStructure = nullptr;
}

void UGA_BuildStructure::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	if (IsLocallyControlled())
	{
		if (TargetDataHandle.IsValid(0))
		{
			if (const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(0))
			{
				if (const FHitResult* TargetHitResult = TargetData->GetHitResult())
				{
					ServerRPCSpawnTargetStructure(TargetStructureClass, TargetHitResult->Location);
				}
			}
		}
	}
}

void UGA_BuildStructure::OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	BuiltStructure = nullptr;
	OnRep_BuiltStructure();
}

void UGA_BuildStructure::ServerRPCSpawnTargetStructure_Implementation(TSubclassOf<class ADFStructureBase> InTargetStructureClass, const FVector_NetQuantize SpawnLocation)
{
	DF_NETGASLOG(LogDFNET, Log, TEXT("Start"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	BuiltStructure = GetWorld()->SpawnActor<ADFStructureBase>(InTargetStructureClass, FTransform(SpawnLocation), SpawnParams);
}

void UGA_BuildStructure::OnRep_BuiltStructure()
{
	DF_NETGASLOG(LogDFGAS, Log, TEXT("Start"));
	if (APawn* AvatarPawn = Cast<APawn>(CurrentActorInfo->AvatarActor))
	{
		if (IPlayerBuildModeInterface* BuildModeInterface = Cast<IPlayerBuildModeInterface>(AvatarPawn->GetController()))
		{
			BuildModeInterface->ExitBuildMode(BuiltStructure.Get());
		}
	}

	TargetStructureClass = nullptr;
	if (BuiltStructure.IsValid())
	{
		BuiltStructure = nullptr;
		K2_EndAbility();
	}
	else
	{
		K2_CancelAbility();
	}
}
