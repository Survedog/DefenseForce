// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS/TA/DFGATA_Trace.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "DFGATA_ProjectilePath.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFGATA_ProjectilePath : public ADFGATA_Trace
{
	GENERATED_BODY()
	
public:
	ADFGATA_ProjectilePath();

protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
	virtual FPredictProjectilePathResult PerformPathPrediction(AActor* InSourceActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	FVector RelativeProjectileSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float ProjectileRadius;

};
