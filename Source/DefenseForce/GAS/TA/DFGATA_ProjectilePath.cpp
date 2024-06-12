// Copyright 2024, Survedog. All rights reserved.


#include "GAS/TA/DFGATA_ProjectilePath.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Structure/DFTowerBase.h"
#include "GAS/TA/Reticle/GAWorldReticle_ProjectilePath.h"
#include "DFLog.h"

ADFGATA_ProjectilePath::ADFGATA_ProjectilePath() : RelativeProjectileSpawnLocation(FVector::Zero())
{
	ProjectileRadius = 0.0f;
}

void ADFGATA_ProjectilePath::OnLaunchVelocityChangedCallback(FVector InLaunchVelocity)
{
	if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
	{
		LaunchVelocity = InLaunchVelocity;
		FPredictProjectilePathResult PredictResult = PerformPathPrediction(SourceActor, InLaunchVelocity, ProjectileRadius);
		TraceHitResult = PredictResult.HitResult;
		FVector EndPoint = TraceHitResult.bBlockingHit ? TraceHitResult.ImpactPoint : PredictResult.LastTraceDestination.Location;

		if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
		{
			LocalReticleActor->SetIsTargetValid(TraceHitResult.bBlockingHit);
			LocalReticleActor->SetIsTargetAnActor(TraceHitResult.GetActor() != nullptr);

			if (AGAWorldReticle_ProjectilePath* DFReticleActor = Cast<AGAWorldReticle_ProjectilePath>(LocalReticleActor))
			{
				DFReticleActor->OnPredictPathResultSet(PredictResult);
			}
			else
			{
				DF_NETLOG(LogDFGAS, Warning, TEXT("Reticle class must be either GAWorldReticle_ProjectilePath or its child class."));
			}
		}

		if (IPlayerTowerControlInterface* PlayerTowerControlInterface = Cast<IPlayerTowerControlInterface>(SourceActor))
		{
			PlayerTowerControlInterface->SetPlayerAimLocation(EndPoint);
		}
	}
}

void ADFGATA_ProjectilePath::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);
}

void ADFGATA_ProjectilePath::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);

	if (ReticleActor.IsValid())
	{
		ReticleActor->SetActorHiddenInGame(false);
	}
}

FHitResult ADFGATA_ProjectilePath::PerformTrace(AActor* InSourceActor)
{
	return PerformPathPrediction(InSourceActor, LaunchVelocity, ProjectileRadius).HitResult;
}

FPredictProjectilePathResult ADFGATA_ProjectilePath::PerformPathPrediction(AActor* InSourceActor, FVector InLaunchVelocity, float InProjectileRadius)
{
	FPredictProjectilePathParams PredictParams;
	PredictParams.TraceChannel = TraceChannel;
	PredictParams.LaunchVelocity = InLaunchVelocity;
	PredictParams.ProjectileRadius = InProjectileRadius;

	IPlayerTowerControlInterface* PlayerTowerControlInterface = CastChecked<IPlayerTowerControlInterface>(InSourceActor);
	ADFTowerBase* ControlledTower = PlayerTowerControlInterface->GetCurrentControlledTower();
	PredictParams.ActorsToIgnore.Add(ControlledTower);
	PredictParams.StartLocation = ControlledTower->GetActorLocation() + RelativeProjectileSpawnLocation;

	if (CollisionFilterMethod == ETargetActorCollisionFilterMethod::CollisionChannel)
	{
		PredictParams.bTraceWithCollision = true;
		PredictParams.bTraceWithChannel = true;
	}
	else
	{
		DF_NETLOG(LogDFGAS, Warning, TEXT("DFGATA_ProjectilePath only supports trace by collision channel."));
	}

#if ENABLE_DRAW_DEBUG
	if (bDebug)
	{
		PredictParams.DrawDebugType = EDrawDebugTrace::ForDuration;
		PredictParams.DrawDebugTime = 0.1f;
	}
#endif // ENABLE_DRAW_DEBUG

	FPredictProjectilePathResult PredictPathResult;
	UGameplayStatics::PredictProjectilePath(InSourceActor->GetWorld(), PredictParams, PredictPathResult);
	return PredictPathResult;
}
