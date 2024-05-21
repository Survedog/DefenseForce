// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFTowerBase.h"
#include "Player/DFPlayerPawn.h"
#include "Net/UnrealNetwork.h"

ADFTowerBase::ADFTowerBase() : ControllingPlayerPawn(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bIsBeingControlled = false;
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

void ADFTowerBase::BeginControl_Implementation(ADFPlayerPawn* NewPlayerPawn)
{
	SetOwner(NewPlayerPawn);
	ControllingPlayerPawn = NewPlayerPawn;
	bIsBeingControlled = true;
}

void ADFTowerBase::EndControl_Implementation()
{
	ControllingPlayerPawn = nullptr;
	bIsBeingControlled = false;
	SetOwner(nullptr);
}

void ADFTowerBase::OnRep_ControllingPlayerPawn_Implementation()
{
	bIsBeingControlled = ControllingPlayerPawn != nullptr ? true : false;
}
