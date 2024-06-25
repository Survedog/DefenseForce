// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFStructureBase.h"
#include "Player/DFPlayerController.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/DFStructureAttributeSet.h"
#include "DFLog.h"

ADFStructureBase::ADFStructureBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->ReplicationMode = EGameplayEffectReplicationMode::Mixed;	

	StructureAttributeSet = CreateDefaultSubobject<UDFStructureAttributeSet>(TEXT("StructureAttributeSet"));

	BuildCost = 0.0f;
}

void ADFStructureBase::AbilityInputPressed(EDFAbilityInputID InputID)
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

void ADFStructureBase::AbilityInputReleased(EDFAbilityInputID InputID)
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

bool ADFStructureBase::TryActivateAbilityOfClass(TSubclassOf<class UGameplayAbility> InAbilityClass, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InAbilityClass);
	if (Spec && !Spec->IsActive())
	{
		return ASC->TryActivateAbility(Spec->Handle, bAllowRemoteActivation);
	}
	return false;
}

int32 ADFStructureBase::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload)
{
	return ASC->HandleGameplayEvent(EventTag, &Payload);
}

void ADFStructureBase::CancelAbilityOfClass(TSubclassOf<class UGameplayAbility> InAbilityClass)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(InAbilityClass);
	if (Spec && Spec->IsActive())
	{
		ASC->CancelAbilityHandle(Spec->Handle);
	}
}

float ADFStructureBase::GetHp() const
{
	return StructureAttributeSet->GetHp();
}

void ADFStructureBase::SetHp(float NewHp)
{
	StructureAttributeSet->SetHp(NewHp);
}

float ADFStructureBase::GetMaxHp() const
{
	return StructureAttributeSet->GetMaxHp();
}

void ADFStructureBase::SetMaxHp(float NewMaxHp)
{
	StructureAttributeSet->SetMaxHp(NewMaxHp);
}

void ADFStructureBase::BeginPlay()
{
	Super::BeginPlay();

	DF_NETLOG(LogDF, Log, TEXT("Start"));
	ADFPlayerController* LocalDFPlayer = Cast<ADFPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (LocalDFPlayer)
	{
		OnBeginCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnBeginCursorOverStructureCallback);
		OnEndCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnEndCursorOverStructureCallback);
	}

	ASC->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		// Give Abilities
		for (auto Ability : NonInputAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
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
