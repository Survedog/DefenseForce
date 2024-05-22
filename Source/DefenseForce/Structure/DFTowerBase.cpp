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

void ADFTowerBase::OnControlStart_Implementation(ADFPlayerPawn* NewPlayerPawn)
{
	ensure(NewPlayerPawn);
	SetOwner(NewPlayerPawn);
	bIsBeingControlled = true;
	ControllingPlayerPawn = NewPlayerPawn;
}

void ADFTowerBase::OnControlEnd_Implementation()
{
	bIsBeingControlled = false;
	ControllingPlayerPawn = nullptr;
	SetOwner(nullptr);
}

void ADFTowerBase::OnRep_ControllingPlayerPawn_Implementation()
{	
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
