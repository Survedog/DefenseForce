// Copyright 2024, Survedog. All rights reserved.


#include "GAS/TA/Reticle/GAWorldReticle_ActorVisualization.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"

AGAWorldReticle_ActorVisualization::AGAWorldReticle_ActorVisualization(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFaceOwner = false;	
	bAllowTickBeforeBeginPlay = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule0"));
	CollisionComponent->InitCapsuleSize(0.f, 0.f);
	CollisionComponent->AlwaysLoadOnClient = true;
	CollisionComponent->SetUsingAbsoluteScale(true);
	CollisionComponent->AlwaysLoadOnServer = true;
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = CollisionComponent;
}

void AGAWorldReticle_ActorVisualization::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	if (bShouldFaceOwner)
	{
		FaceTowardSource(bFaceOwnerFlat);
	}
}


void AGAWorldReticle_ActorVisualization::InitializeVisualizationInformation(TSubclassOf<AActor> PlacedActorClass, UMaterialInterface* VisualizationMaterial)
{
	if (PlacedActorClass)
	{
		if (AActor* VisualizationActor = GetWorld()->SpawnActor<AActor>(PlacedActorClass))
		{
			//Get components
			TInlineComponentArray<UMeshComponent*> MeshComps;
			USceneComponent* MyRoot = GetRootComponent();
			VisualizationActor->GetComponents(MeshComps);
			check(MyRoot);
			
			AddTickPrerequisiteActor(TargetingActor);		//We want the reticle to tick after the targeting actor so that designers have the final say on the position
			VisualizationActor->SetRootComponent(NULL); //If we don't clear the root component explicitly, the component will be destroyed along with the original visualization actor.

			for (UMeshComponent* MeshComp : MeshComps)
			{
				//Disable collision on visualization mesh parts so it doesn't interfere with aiming or any other client-side collision/prediction/physics stuff
				MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);		//All mesh components are primitive components, so no cast is needed

				//Move components from one actor to the other, attaching as needed. Hierarchy should not be important, but we can do fixups if it becomes important later.
				MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				MeshComp->AttachToComponent(MyRoot, FAttachmentTransformRules::KeepRelativeTransform);
				MeshComp->Rename(nullptr, this);
				if (VisualizationMaterial)
				{
					int32 MaterialCount = MeshComp->GetMaterials().Num();
					for (int32 Idx = 0; Idx < MaterialCount; ++Idx)
					{
						MeshComp->SetMaterial(Idx, VisualizationMaterial);
					}
				}
			}

			VisualizationActor->Destroy();
		}
	}
}