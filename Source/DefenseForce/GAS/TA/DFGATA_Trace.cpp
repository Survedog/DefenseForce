// Copyright 2024, Survedog. All rights reserved.


#include "GAS/TA/DFGATA_Trace.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemLog.h"
#include "GAS/TA/Reticle/DFGAWorldReticle.h" 
#include "Interface/PlayerTowerControlInterface.h"
#include "DFLog.h"

ADFGATA_Trace::ADFGATA_Trace()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	MaxRange = 999999.0f;
	bIgnoreBlockingHits = false;
	bTraceStartsFromPlayerCamera = true;
	bTraceTowardMouseAimLocation = true;

	CollisionFilterMethod = ETargetActorCollisionFilterMethod::CollisionProfile;
	bIsTraceHit = false;
}

void ADFGATA_Trace::LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (const FHitResult& Hit : HitResults)
	{
		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

void ADFGATA_Trace::LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, ECollisionChannel CollisionChannel, const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->LineTraceMultiByChannel(HitResults, Start, End, CollisionChannel, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (const FHitResult& Hit : HitResults)
	{
		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

void ADFGATA_Trace::SweepWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->SweepMultiByProfile(HitResults, Start, End, Rotation, ProfileName, CollisionShape, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (const FHitResult& Hit : HitResults)
	{
		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

bool ADFGATA_Trace::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DotToCenter >= 0)		//If this fails, we're pointed away from the center, but we might be inside the sphere and able to find a good exit point.
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;						//Subtracting instead of adding will get the other intersection point
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);		//Cam aim point clipped to range sphere
			return true;
		}
	}
	return false;
}

void ADFGATA_Trace::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	DF_NETLOG(LogDFGAS, Log, TEXT("start"));

	SetActorTickEnabled(true);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
		
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = nullptr;
		if (ReticleActor.IsValid() && ReticleActor->GetClass() == ReticleClass)
		{
			SpawnedReticleActor = ReticleActor.Get();
		}
		else
		{
			if (ReticleActor.IsValid())
			{
				ReticleActor->Destroy();
			}
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		}

		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams); // TODO: Check if it's okay to initialize an reused reticle actor again.
			ReticleActor = SpawnedReticleActor;

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistancy between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isnt a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}
		}
	}

	OnStartTargeting(InAbility);
}

void ADFGATA_Trace::ConfirmTargeting()
{
	Super::ConfirmTargeting();

	SetActorTickEnabled(false);
	if (ReticleActor.IsValid())
	{
		ReticleActor->SetActorHiddenInGame(true);
	}
}

void ADFGATA_Trace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	DF_NETLOG(LogDFGAS, Log, TEXT("start"));

	if (SourceActor && IsConfirmTargetingAllowed())
	{
		//bDebug = false;
		//FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformTrace(SourceActor));
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(TraceHitResult);
		TargetDataReadyDelegate.Broadcast(Handle);
		OnConfirmTargeting();
	}
}

void ADFGATA_Trace::CancelTargeting()
{
	DF_NETLOG(LogDFGAS, Log, TEXT("start"));
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* ASC = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (ASC)
	{
		ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GenericCancel, OwningAbility->GetCurrentAbilitySpecHandle(), OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()).Remove(GenericCancelHandle);
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"), *GetName());
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	SetActorTickEnabled(false);

	if (ReticleActor.IsValid())
	{
		ReticleActor->SetActorHiddenInGame(true);
	}

	OnCancelTargeting();
}

void ADFGATA_Trace::BeginPlay()
{
	Super::BeginPlay();

	DF_NETLOG(LogDFGAS, Log, TEXT("start"));
	SetActorTickEnabled(false);
}

void ADFGATA_Trace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DF_NETLOG(LogDFGAS, Log, TEXT("start"));
	if (ReticleActor.IsValid())
	{
		ReticleActor->Destroy();
	}

	Super::EndPlay(EndPlayReason);
}

void ADFGATA_Trace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// very temp - do a mostly hardcoded trace from the source actor
	if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
	{
		TraceHitResult = PerformTrace(SourceActor);
		FVector EndPoint = TraceHitResult.Component.IsValid() ? TraceHitResult.ImpactPoint : TraceHitResult.TraceEnd;

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			DrawDebugLine(GetWorld(), SourceActor->GetActorLocation(), EndPoint, FColor::Green, false);
			DrawDebugSphere(GetWorld(), EndPoint, 16, 10, FColor::Green, false);
		}
#endif // ENABLE_DRAW_DEBUG

		if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
		{
			if (TraceHitResult.bBlockingHit)
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
					LocalReticleActor->SetActorLocation(TraceHitResult.Location);
				}
			}
			else
			{
				LocalReticleActor->SetActorHiddenInGame(true);
				LocalReticleActor->SetIsTargetValid(false);
				LocalReticleActor->SetIsTargetAnActor(false);
			}
		}
		
		if (IPlayerTowerControlInterface* PlayerTowerControlInterface = Cast<IPlayerTowerControlInterface>(SourceActor))
		{
			PlayerTowerControlInterface->SetPlayerAimLocation(EndPoint);
		}

		SetActorLocationAndRotation(EndPoint, SourceActor->GetActorRotation());
	}
}

void ADFGATA_Trace::StopTargeting()
{
	DF_NETLOG(LogDFGAS, Log, TEXT("start"));
	SetActorTickEnabled(false);

	if (ReticleActor.IsValid())
	{
		ReticleActor->SetActorHiddenInGame(true);
	}

	TargetDataReadyDelegate.Clear();
	CanceledDelegate.Clear();
	
	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
		GenericDelegateBoundASC = nullptr;
	}

	OnStopTargeting();
}

FHitResult ADFGATA_Trace::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ADFGATA_Trace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(InSourceActor);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();// InSourceActor->GetActorLocation();
	FVector TraceEnd = FVector::Zero();
	
	if (PrimaryPC)
	{
		FVector PlayerViewLocation;
		FRotator PlayerViewRotation;
		PrimaryPC->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

		if (bTraceStartsFromPlayerCamera)
		{
			TraceStart = PlayerViewLocation;
		}

		if (bTraceTowardMouseAimLocation)
		{
			FVector MouseWorldLocation, MouseWorldDirection;
			PrimaryPC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
			
			FVector TraceDirection = (MouseWorldLocation - TraceStart).GetSafeNormal();
			TraceEnd = TraceDirection.IsNearlyZero() ? PlayerViewLocation + PlayerViewRotation.Vector() * MaxRange : TraceStart + TraceDirection * MaxRange;
		}
		else
		{
			TraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * MaxRange;
		}
	}

	// ------------------------------------------------------

	FHitResult ReturnHitResult;
	//Use a line trace initially to see where the player is actually pointing
	if (CollisionFilterMethod == ETargetActorCollisionFilterMethod::CollisionProfile)
	{
		LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
	}
	else
	{
		LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceChannel, Params);
	}

	//Default to end of trace line if we don't hit anything.
	if (!ReturnHitResult.bBlockingHit)
	{
		ReturnHitResult.Location = TraceEnd;
	}

	// Reset the trace start so the target data uses the correct origin
	ReturnHitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();

	return ReturnHitResult;
}

FGameplayAbilityTargetDataHandle ADFGATA_Trace::MakeTargetData(const FHitResult& HitResult) const
{
	/** Note: This will be cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	return StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
}
