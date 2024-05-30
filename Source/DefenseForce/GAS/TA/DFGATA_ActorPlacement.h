// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "DFGATA_ActorPlacement.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFGATA_ActorPlacement : public ADFGATA_Trace
{
	GENERATED_BODY()

public:
	ADFGATA_ActorPlacement();

	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	FORCEINLINE TSubclassOf<class AActor> GetPlacedActorClass() const { return PlacedActorClass; }
	FORCEINLINE void SetPlacedActorClass(TSubclassOf<class AActor> InPlacedActorClass) { PlacedActorClass = InPlacedActorClass; }

	FORCEINLINE UMaterialInterface* GetPlacedActorMaterial() const { return PlacedActorMaterial; }
	FORCEINLINE void SetPlacedActorMaterial(class UMaterialInterface* InPlacedActorMaterial) { PlacedActorMaterial = InPlacedActorMaterial; }

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	TSubclassOf<class AActor> PlacedActorClass;

	/** Override Material 0 on our placed actor's meshes with this material for visualization. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	TObjectPtr<class UMaterialInterface> PlacedActorMaterial;
};
