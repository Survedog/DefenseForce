// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/DFInputGameplayAbility.h"
#include "GA_TowerAttack.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UGA_TowerAttack : public UDFInputGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_TowerAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TSubclassOf<class ADFGATA_Trace> TargetActorClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Targeting")
	TObjectPtr<class ADFGATA_Trace> DFTargetActor;

	/** Which reticle class to use for targeting. Overrides reticle class field of target actor. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TSubclassOf<class AGameplayAbilityWorldReticle> ReticleClass;

};
