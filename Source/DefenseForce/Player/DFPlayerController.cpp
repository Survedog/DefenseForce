// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerController.h"
#include "Player/DFPlayerPawn.h"
#include "AbilitySystemComponent.h"
#include "Structure/DFTowerBase.h"

ADFPlayerController::ADFPlayerController() : DFPlayerPawn(nullptr), CurrentControlledTower(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

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
		CurrentControlledTower->BeginControl(DFPlayerPawn);
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

void ADFPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	DFPlayerPawn = CastChecked<ADFPlayerPawn>(aPawn);
}
