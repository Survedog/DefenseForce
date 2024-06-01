// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerController.h"
#include "Player/DFPlayerPawn.h"
#include "AbilitySystemComponent.h"
#include "Structure/DFTowerBase.h"
#include "Net/UnrealNetwork.h"
#include "DFLog.h"

ADFPlayerController::ADFPlayerController() : DFPlayerPawn(nullptr), CurrentStructureUnderCursor(nullptr), CurrentControlledTower(nullptr)
{
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ADFPlayerController::StartTowerControl_Implementation(ADFTowerBase* NewTower)
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	if (!NewTower->IsBeingControlled())
	{
		if (CurrentControlledTower)
		{
			EndTowerControl();
		}
		CurrentControlledTower = NewTower;
		CurrentControlledTower->OnControlStart(DFPlayerPawn);
		OnTowerControlStart.Broadcast(NewTower);
	}
}

void ADFPlayerController::EndTowerControl_Implementation()
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	if (CurrentControlledTower && CurrentControlledTower->IsBeingControlled())
	{
		CurrentControlledTower->OnControlEnd();
		OnTowerControlEnd.Broadcast(CurrentControlledTower);
		CurrentControlledTower = nullptr;
	}
}

void ADFPlayerController::OnRep_CurrentControlledTower()
{
	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	if (CurrentControlledTower)
	{
		OnTowerControlStart.Broadcast(CurrentControlledTower);
	}
	else
	{
		OnTowerControlEnd.Broadcast(CurrentControlledTower);
	}
}

void ADFPlayerController::OnBeginCursorOverStructureCallback_Implementation(AActor* TouchedActor)
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	CurrentStructureUnderCursor = Cast<ADFStructureBase>(TouchedActor);
}

void ADFPlayerController::OnEndCursorOverStructureCallback_Implementation(AActor* TouchedActor)
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	if (CurrentStructureUnderCursor == TouchedActor)
	{
		CurrentStructureUnderCursor = nullptr;
	}
}

void ADFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DF_NETLOG(LogDF, Log, TEXT("Start"));
}

void ADFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	DFPlayerPawn = CastChecked<ADFPlayerPawn>(aPawn);
}

void ADFPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	DFPlayerPawn = CastChecked<ADFPlayerPawn>(P);
}

void ADFPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADFPlayerController, CurrentControlledTower, COND_AutonomousOnly);
}
