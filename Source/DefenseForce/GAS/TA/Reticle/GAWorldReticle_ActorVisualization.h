// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "GAWorldReticle_ActorVisualization.generated.h"

/**
 * Custom class of AGameplayAbilityWorldReticle_ActorVisualization.
 */
UCLASS(NotPlaceable)
class DEFENSEFORCE_API AGAWorldReticle_ActorVisualization : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()

public:
	AGAWorldReticle_ActorVisualization(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	void InitializeVisualizationInformation(TSubclassOf<AActor> PlacedActorClass, UMaterialInterface* VisualizationMaterial);

	/** Returns CollisionComponent subobject **/
	class UCapsuleComponent* GetCollisionComponent() const { return CollisionComponent; }

protected:
	/** Should it keep rotating to face owner? Tick function must be enabled to use this functionality. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	uint8 bShouldFaceOwner : 1;

private:
	/** Hardcoded collision component, so other objects don't think they can collide with the visualization actor */
	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> CollisionComponent;
};
