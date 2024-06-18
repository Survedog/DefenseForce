// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFStructureBase.h"
#include "Player/DFPlayerController.h"
#include "AbilitySystemComponent.h"
#include "DFLog.h"

ADFStructureBase::ADFStructureBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	BuildCost = 0.0f;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->ReplicationMode = EGameplayEffectReplicationMode::Mixed;
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
		for (auto ActivatableAbility : ActivatableAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(ActivatableAbility);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}
