// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerPawn.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "Player/DFPlayerController.h"
#include "AbilitySystemComponent.h"
#include "DefenseForce.h"

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

	DFPlayerController = CastChecked<ADFPlayerController>(NewController);
	DFPlayerController->OnTowerControlStart.AddDynamic(this, &ADFPlayerPawn::OnTowerControlStartCallback);
	DFPlayerController->OnTowerControlEnd.AddDynamic(this, &ADFPlayerPawn::OnTowerControlEndCallback);

	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(NewController->PlayerState);
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
		ensure(ASC.Get());
		ASC->InitAbilityActorInfo(NewController->PlayerState, this);

		// Give Abilities
		for (auto NonInputAbility : NonInputAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(NonInputAbility);
			ASC->GiveAbility(AbilitySpec);
		}

		for (auto InputAbilityPair : InputAbilityMap)
		{
			FGameplayAbilitySpec AbilitySpec(InputAbilityPair.Value);
			AbilitySpec.InputID = static_cast<int32>(InputAbilityPair.Key);
			ASC->GiveAbility(AbilitySpec);
		}
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
		
		// Give Abilities
		for (auto NonInputAbility : NonInputAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(NonInputAbility);
			ASC->GiveAbility(AbilitySpec);
		}

		for (auto InputAbilityPair : InputAbilityMap)
		{
			FGameplayAbilitySpec AbilitySpec(InputAbilityPair.Value);
			AbilitySpec.InputID = static_cast<int32>(InputAbilityPair.Key);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void ADFPlayerPawn::AbilityInputPressed(EDFAbilityInputID InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputID));
	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle, true);
		}
	}
}

void ADFPlayerPawn::AbilityInputReleased(EDFAbilityInputID InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(static_cast<int32>(InputID));
	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void ADFPlayerPawn::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (Controller)
	{
		DFPlayerController = CastChecked<ADFPlayerController>(Controller);
		DFPlayerController->OnTowerControlStart.AddDynamic(this, &ADFPlayerPawn::OnTowerControlStartCallback);
		DFPlayerController->OnTowerControlEnd.AddDynamic(this, &ADFPlayerPawn::OnTowerControlEndCallback);
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
