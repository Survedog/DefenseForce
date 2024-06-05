// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFTowerBase.h"
#include "Player/DFPlayerPawn.h"
#include "Net/UnrealNetwork.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "DFLog.h"

ADFTowerBase::ADFTowerBase() : ControllingPlayerPawn(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bIsBeingControlled = false;
	bConfirmTargetOnInputPressed = true;

	static ConstructorHelpers::FClassFinder<ADFGATA_Trace> TargetActorClassRef(TEXT("/Script/CoreUObject.Class'/Script/DefenseForce.DFGATA_Trace'"));
	if (TargetActorClassRef.Class)
	{
		TargetActorClass = TargetActorClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameplayAbilityWorldReticle> ReticleClassRef(TEXT("/Game/DefenseForce/Blueprint/GAS/TA/ReticleActor/BP_GAWorldReticle_AimArrow.BP_GAWorldReticle_AimArrow_C"));
	if (ReticleClassRef.Class)
	{
		ReticleClass = ReticleClassRef.Class;
	}
}

bool ADFTowerBase::IsLocallyControlled() const
{
	if (ControllingPlayerPawn.IsValid())
	{
		return ControllingPlayerPawn->IsLocallyControlled();
	}
	return false;
}

void ADFTowerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFTowerBase, ControllingPlayerPawn);
}

void ADFTowerBase::OnControlStart_Implementation(ADFPlayerPawn* NewPlayerPawn)
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	ensure(NewPlayerPawn);
	SetOwner(NewPlayerPawn);
	bIsBeingControlled = true;
	ControllingPlayerPawn = NewPlayerPawn;
}

void ADFTowerBase::OnControlEnd_Implementation()
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	bIsBeingControlled = false;
	ControllingPlayerPawn = nullptr;
	SetOwner(nullptr);
}

void ADFTowerBase::OnRep_ControllingPlayerPawn_Implementation()
{	
	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	if (ControllingPlayerPawn.IsValid())
	{
		OnControlStart(ControllingPlayerPawn.Get());
	}
	else
	{
		if (bIsBeingControlled)
		{
			OnControlEnd();
		}
	}
}
