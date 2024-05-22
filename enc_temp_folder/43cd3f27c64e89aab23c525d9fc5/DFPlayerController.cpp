// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerController.h"
#include "Player/DFPlayerPawn.h"
#include "AbilitySystemComponent.h"
#include "Structure/DFTowerBase.h"
#include "Net/UnrealNetwork.h"

ADFPlayerController::ADFPlayerController() : DFPlayerPawn(nullptr), CurrentControlledTower(nullptr)
{
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ADFPlayerController::StartTowerControl(ADFTowerBase* NewTower)
{
	if (!NewTower->IsBeingControlled())
	{
		if (CurrentControlledTower.IsValid())
		{
			EndTowerControl();
		}
		CurrentControlledTower = NewTower;
		CurrentControlledTower->OnControlStart(DFPlayerPawn);
		OnTowerControlStart.Broadcast(NewTower);
	}
}

void ADFPlayerController::EndTowerControl()
{
	if (CurrentControlledTower.IsValid() && CurrentControlledTower->IsBeingControlled())
	{
		CurrentControlledTower->OnControlEnd();
		OnTowerControlEnd.Broadcast(CurrentControlledTower.Get());
		CurrentControlledTower = nullptr;
	}
}

void ADFPlayerController::OnRep_CurrentControlledTower()
{
	if (CurrentControlledTower.IsValid())
	{
		OnTowerControlStart.Broadcast(CurrentControlledTower.Get());
	}
	else
	{
		OnTowerControlEnd.Broadcast(CurrentControlledTower.Get());
	}
}

void ADFPlayerController::OnBeginCursorOverStructureCallback_Implementation(AActor* TouchedActor)
{
	CurrentStructureUnderCursor = Cast<ADFStructureBase>(TouchedActor);
}

void ADFPlayerController::OnEndCursorOverStructureCallback_Implementation(AActor* TouchedActor)
{
	if (CurrentStructureUnderCursor == TouchedActor)
	{
		CurrentStructureUnderCursor = nullptr;
	}
}

void ADFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	DFPlayerPawn = CastChecked<ADFPlayerPawn>(aPawn);
}

void ADFPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	DFPlayerPawn = CastChecked<ADFPlayerPawn>(P);
}

void ADFPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADFPlayerController, CurrentControlledTower, COND_SimulatedOnly);
}
