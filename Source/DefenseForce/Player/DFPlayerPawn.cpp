// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerPawn.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"

ADFPlayerPawn::ADFPlayerPawn() : PlayerAimLocation(FVector::Zero())
{
	PrimaryActorTick.bCanEverTick = false;

	PlayerAimTraceLength = 100000.0f;
}

void ADFPlayerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADFPlayerPawn, PlayerAimLocation, COND_SimulatedOnly);
}

void ADFPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(NewController->PlayerState);
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
		ensure(ASC.Get());
		ASC->InitAbilityActorInfo(NewController->PlayerState, this);
	}
}

void ADFPlayerPawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
		ensure(ASC.Get());
		ASC->InitAbilityActorInfo(GetPlayerState(), this);
	}
}

void ADFPlayerPawn::OnRep_PlayerAimLocation()
{
	UE_LOG(LogTemp, Log, TEXT("[%s]: OnRep_PlayerAimLocation"), GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));
}

UAbilitySystemComponent* ADFPlayerPawn::GetAbilitySystemComponent() const
{
	return ASC.Get();
}
