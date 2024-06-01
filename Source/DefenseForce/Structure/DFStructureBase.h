// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFStructureBase.generated.h"

UCLASS()
class DEFENSEFORCE_API ADFStructureBase : public AActor
{
	GENERATED_BODY()
	
public:
	ADFStructureBase();

	FORCEINLINE float GetBuildCost() const { return BuildCost; }
	FORCEINLINE void SetBuildCost(float NewBuildCost) { BuildCost = NewBuildCost; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BuildCost;
};
