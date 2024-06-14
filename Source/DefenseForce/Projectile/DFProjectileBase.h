// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFProjectileBase.generated.h"

UCLASS()
class DEFENSEFORCE_API ADFProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFProjectileBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	FORCEINLINE float ApplyPointDamage(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo, TSubclassOf<UDamageType> DamageTypeClass);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	FORCEINLINE bool ApplyRadialDamage(const UObject* WorldContextObject, float BaseDamage, const FVector& Origin, float DamageRadius, TSubclassOf<class UDamageType> DamageTypeClass, const TArray<AActor*>& IgnoreActors, bool bDoFullDamage = false, ECollisionChannel DamagePreventionChannel = ECC_Visibility);
};
