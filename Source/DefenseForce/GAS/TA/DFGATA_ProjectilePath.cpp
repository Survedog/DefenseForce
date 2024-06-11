// Copyright 2024, Survedog. All rights reserved.


#include "GAS/TA/DFGATA_ProjectilePath.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Structure/DFTowerBase.h"
#include "GAS/TA/Reticle/DFGAWorldReticle.h"
#include "DFLog.h"

ADFGATA_ProjectilePath::ADFGATA_ProjectilePath() : RelativeProjectileSpawnLocation(FVector::Zero())
{
	ProjectileRadius = 0.0f;
}

void ADFGATA_ProjectilePath::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
	{
		FPredictProjectilePathResult PredictResult = PerformPathPrediction(SourceActor);
		TraceHitResult = PredictResult.HitResult;
		FVector EndPoint = TraceHitResult.bBlockingHit ? TraceHitResult.ImpactPoint : PredictResult.LastTraceDestination.Location;

		if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
		{
			LocalReticleActor->SetActorHiddenInGame(false);
			LocalReticleActor->SetIsTargetValid(true);
			LocalReticleActor->SetIsTargetAnActor(true);

			if (ADFGAWorldReticle* DFReticleActor = Cast<ADFGAWorldReticle>(LocalReticleActor))
			{
				DFReticleActor->OnTraceResultSet(TraceHitResult);
			}
			else
			{
				DF_NETLOG(LogDFGAS, Warning, TEXT("Reticle class must be either DFGAWorldReticle or its child class."));
			}
		}

		if (IPlayerTowerControlInterface* PlayerTowerControlInterface = Cast<IPlayerTowerControlInterface>(SourceActor))
		{
			PlayerTowerControlInterface->SetPlayerAimLocation(EndPoint);
		}

		SetActorLocationAndRotation(EndPoint, SourceActor->GetActorRotation());
	}
}

FHitResult ADFGATA_ProjectilePath::PerformTrace(AActor* InSourceActor)
{
	return PerformPathPrediction(InSourceActor).HitResult;
}

FPredictProjectilePathResult ADFGATA_ProjectilePath::PerformPathPrediction(AActor* InSourceActor)
{
	FPredictProjectilePathResult PredictPathResult;

	FPredictProjectilePathParams PredictParams;
	PredictParams.ProjectileRadius = ProjectileRadius;
	PredictParams.TraceChannel = TraceChannel;

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
	UGameplayStatics::PredictProjectilePath(InSourceActor->GetWorld(), PredictParams, PredictPathResult);
	return PredictPathResult;
}
