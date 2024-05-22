// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "DefenseForce.h"
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
	UFUNCTION(BlueprintCallable)
	void AbilityInputPressed(EDFAbilityInputID InputID);

	UFUNCTION(BlueprintCallable)
	void AbilityInputReleased(EDFAbilityInputID InputID);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TWeakObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TMap<EDFAbilityInputID, TSubclassOf<class UGameplayAbility>> InputAbilityMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> NonInputAbilities;

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
