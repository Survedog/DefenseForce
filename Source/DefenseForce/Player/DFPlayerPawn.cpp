// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerPawn.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"

ADFPlayerPawn::ADFPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADFPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(NewController->PlayerState);
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
	}
}

void ADFPlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
	}
}

UAbilitySystemComponent* ADFPlayerPawn::GetAbilitySystemComponent() const
{
	return ASC.Get();
}
