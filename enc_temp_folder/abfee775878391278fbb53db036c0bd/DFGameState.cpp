// Copyright 2024, Survedog. All rights reserved.


#include "Game/DFGameState.h"
#include "Net/UnrealNetwork.h"

ADFGameState::ADFGameState()
{
	CurrentMoneyAmount = 0.0f;
}

void ADFGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFGameState, CurrentMoneyAmount);
}

void ADFGameState::OnRep_CurrentMoneyAmount()
{
	OnCurrentMoneyAmountChange.Broadcast(CurrentMoneyAmount);
}
