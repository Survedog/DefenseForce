// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "DFGATA_Trace.generated.h"

UENUM(BlueprintType)
enum class ETargetActorCollisionFilterMethod : uint8
{
	CollisionProfile,
	CollisionChannel
};

/**
 *  Custom reusable version of AGameplayAbilityTargetActor_Trace.
 *  This target actor reuses reticle actor, avoiding duplicate spawning of it.
 *  Meant to be used with AbilityTask DFAT_WaitTargetData_ReusableTA.
 */
UCLASS()
class DEFENSEFORCE_API ADFGATA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	ADFGATA_Trace();

	/** Traces as normal, but will manually filter all hit actors */
	static void LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);
	static void LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, ECollisionChannel CollisionChannel, const FCollisionQueryParams Params);

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

	FORCEINLINE void SetTraceProfile(FName InProfileName) { TraceProfile = FCollisionProfileName(InProfileName); CollisionFilterMethod = ETargetActorCollisionFilterMethod::CollisionProfile; }
	FORCEINLINE void SetTraceChannel(ECollisionChannel InCollisionChannel) { TraceChannel = InCollisionChannel; CollisionFilterMethod = ETargetActorCollisionFilterMethod::CollisionChannel; }
	FORCEINLINE void SetCollisionFilterMethod(ETargetActorCollisionFilterMethod InCollisionFilterMethod) { CollisionFilterMethod = InCollisionFilterMethod; }

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = "Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	ETargetActorCollisionFilterMethod CollisionFilterMethod;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Trace")
	FHitResult TraceHitResult;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Targeting")
	TWeakObjectPtr<AGameplayAbilityWorldReticle> ReticleActor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Targeting")
	uint8 bIsTraceHit : 1;
};
