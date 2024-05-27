// Copyright 2024, Survedog. All rights reserved.


#include "Player/DFPlayerPawn.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "Player/DFPlayerController.h"
#include "AbilitySystemComponent.h"
#include "DefenseForce.h"
#include "DFLog.h"

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

	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
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

	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	IAbilitySystemInterface* PSGASInterface = Cast<IAbilitySystemInterface>(GetPlayerState());
	if (PSGASInterface)
	{
		ASC = PSGASInterface->GetAbilitySystemComponent();
		ensure(ASC.Get());
		ASC->InitAbilityActorInfo(GetPlayerState(), this);
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

bool ADFPlayerPawn::TryActivateAbilityOfClass(TSubclassOf<UGameplayAbility> InAbilityClass, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InAbilityClass);
	if (Spec && !Spec->IsActive())
	{
		return ASC->TryActivateAbilityByClass(InAbilityClass, bAllowRemoteActivation);
	}
	return false;
}

void ADFPlayerPawn::CancelAbilityOfClass(TSubclassOf<UGameplayAbility> InAbilityClass)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InAbilityClass);
	if (Spec && Spec->IsActive())
	{
		ASC->CancelAbilityHandle(Spec->Handle);
	}
}

void ADFPlayerPawn::OnRep_Controller()
{
	Super::OnRep_Controller();

	DF_NETLOG(LogDFNET, Log, TEXT("Start"));
	if (Controller && Controller != DFPlayerController)
	{
		DFPlayerController = CastChecked<ADFPlayerController>(Controller);
		DFPlayerController->OnTowerControlStart.AddUniqueDynamic(this, &ADFPlayerPawn::OnTowerControlStartCallback);
		DFPlayerController->OnTowerControlEnd.AddUniqueDynamic(this, &ADFPlayerPawn::OnTowerControlEndCallback);
	}
}

UAbilitySystemComponent* ADFPlayerPawn::GetAbilitySystemComponent() const
{
	return ASC.Get();
}
