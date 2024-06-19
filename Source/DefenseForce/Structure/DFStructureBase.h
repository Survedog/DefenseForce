// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "DFStructureBase.generated.h"

UCLASS()
class DEFENSEFORCE_API ADFStructureBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADFStructureBase();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

	FORCEINLINE float GetBuildCost() const { return BuildCost; }
	FORCEINLINE void SetBuildCost(float NewBuildCost) { BuildCost = NewBuildCost; }

	UFUNCTION(BlueprintCallable)
	bool TryActivateAbilityOfClass(TSubclassOf<class UGameplayAbility> InAbilityClass, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData& Payload);

	UFUNCTION(BlueprintCallable)
	void CancelAbilityOfClass(TSubclassOf<class UGameplayAbility> InAbilityClass);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Build")
	float BuildCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> ActivatableAbilities;
};
