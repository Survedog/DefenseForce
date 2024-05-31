// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_BuildStructure.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UGA_BuildStructure : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_BuildStructure();

protected:
	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION()
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Build")
	TSubclassOf<class ADFStructureBase> PlacedStructureClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TSubclassOf<class ADFGATA_ActorPlacement> ActorPlacementTAClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Targeting")
	TObjectPtr<class ADFGATA_ActorPlacement> DFActorPlacementTA;	

	/** Which reticle class to use for targeting. Overrides reticle class field of target actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TSubclassOf<class AGAWorldReticle_ActorVisualization> ActorVisualReticleClass;
};
