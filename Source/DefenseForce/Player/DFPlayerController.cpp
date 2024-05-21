// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerController.h"
#include "Player/DFPlayerPawn.h"
#include "AbilitySystemComponent.h"
#include "Structure/DFTowerBase.h"

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
		CurrentControlledTower->BeginControl(this);
	}
}

void ADFPlayerController::EndTowerControl()
{
	if (CurrentControlledTower.IsValid() && CurrentControlledTower->IsBeingControlled())
	{
		CurrentControlledTower->EndControl();
		CurrentControlledTower = nullptr;
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
