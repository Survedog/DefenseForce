// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "DFGATA_Trace.generated.h"

/**
 *  Custom version of AGameplayAbilityTargetActor_Trace.
 */
UCLASS()
class DEFENSEFORCE_API ADFGATA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ADFGATA_Trace();

	/** Traces as normal, but will manually filter all hit actors */
	static void LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);

	/** Sweeps as normal, but will manually filter all hit actors */
	static void SweepWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params);

	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);
	
	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	virtual void ConfirmTargeting() override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void CancelTargeting() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void StopTargeting();

	UFUNCTION(BlueprintNativeEvent)
	void OnStartTargeting(UGameplayAbility* InAbility);

	UFUNCTION(BlueprintNativeEvent)
	void OnConfirmTargeting();

	UFUNCTION(BlueprintNativeEvent)
	void OnCancelTargeting();

	UFUNCTION(BlueprintNativeEvent)
	void OnStopTargeting();

	void OnStartTargeting_Implementation(UGameplayAbility* InAbility) {}
	void OnConfirmTargeting_Implementation() {}
	void OnCancelTargeting_Implementation() {}
	void OnStopTargeting_Implementation() {}

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	float MaxRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bIgnoreBlockingHits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceStartsFromPlayerCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceTowardMouseAimLocation;

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor);

	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Targeting")
	TWeakObjectPtr<AGameplayAbilityWorldReticle> ReticleActor;
};
