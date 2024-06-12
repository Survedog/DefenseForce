// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/TA/DFGATA_LineTrace.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DFGATA_ProjectilePath.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFGATA_ProjectilePath : public ADFGATA_LineTrace
{
	GENERATED_BODY()
	
public:
	ADFGATA_ProjectilePath();

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	virtual FPredictProjectilePathResult PerformPathPrediction(class ADFTowerBase* InControlledTower, FVector InStartLocation, FVector InLaunchVelocity, float InProjectileRadius = 0.0f);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float ProjectileRadius;
};
