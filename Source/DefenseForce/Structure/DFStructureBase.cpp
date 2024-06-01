// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFStructureBase.h"
#include "Player/DFPlayerController.h"
#include "DFLog.h"

ADFStructureBase::ADFStructureBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	BuildCost = 0.0f;
}

void ADFStructureBase::BeginPlay()
{
	Super::BeginPlay();

	DF_NETLOG(LogDF, Log, TEXT("Start"));
	ADFPlayerController* LocalDFPlayer = Cast<ADFPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (LocalDFPlayer)
	{
		OnBeginCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnBeginCursorOverStructureCallback);
		OnEndCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnEndCursorOverStructureCallback);
	}
}
