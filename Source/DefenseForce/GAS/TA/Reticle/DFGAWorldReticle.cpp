// Copyright 2024, Survedog. All rights reserved.


#include "GAS/TA/Reticle/DFGAWorldReticle.h"

ADFGAWorldReticle::ADFGAWorldReticle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFaceOwner = false;
	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ADFGAWorldReticle::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	if (bShouldFaceOwner)
	{
		FaceTowardSource(bFaceOwnerFlat);
	}
}
