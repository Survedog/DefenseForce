// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "DFPlayerPawn.generated.h"

UCLASS()
class DEFENSEFORCE_API ADFPlayerPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADFPlayerPawn();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	//virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState();
	

// GAS
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TWeakObjectPtr<class UAbilitySystemComponent> ASC;

// Player Control
protected:
	UFUNCTION()
	void OnRep_PlayerAimLocation();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_PlayerAimLocation, Category = "Player Control")
	FVector PlayerAimLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Control")
	float PlayerAimTraceLength;
};
