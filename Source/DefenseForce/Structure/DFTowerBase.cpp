// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFTowerBase.h"
#include "Player/DFPlayerController.h"
#include "Net/UnrealNetwork.h"

ADFTowerBase::ADFTowerBase() : ControllingPlayer(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	bIsBeingControlled = false;
}

void ADFTowerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ADFTowerBase, ControllingPlayer);
}

void ADFTowerBase::BeginControl_Implementation(ADFPlayerController* NewControllingPlayer)
{
	SetOwner(NewControllingPlayer);
	ControllingPlayer = NewControllingPlayer;
	bIsBeingControlled = true;
}

void ADFTowerBase::EndControl_Implementation()
{
	ControllingPlayer = nullptr;
	bIsBeingControlled = false;
	SetOwner(nullptr);
}

//void ADFTowerBase::OnRep_ControllingPlayer_Implementation()
//{
//	bIsBeingControlled = ControllingPlayer != nullptr ? true : false;
//}
