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

protected:
	virtual void BeginPlay() override;	
};
