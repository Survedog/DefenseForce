// Copyright 2024, Survedog. All rights reserved.


#include "Structure/DFStructureBase.h"
#include "Player/DFPlayerController.h"

ADFStructureBase::ADFStructureBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ADFStructureBase::BeginPlay()
{
	Super::BeginPlay();

	ADFPlayerController* LocalDFPlayer = Cast<ADFPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (LocalDFPlayer)
	{
		OnBeginCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnBeginCursorOverStructureCallback);
		OnEndCursorOver.AddDynamic(LocalDFPlayer, &ADFPlayerController::OnEndCursorOverStructureCallback);
	}
}
