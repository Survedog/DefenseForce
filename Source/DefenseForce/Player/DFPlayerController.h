// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "DFPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlStartDelegate, ADFTowerBase*, NewControlledTower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlEndDelegate, ADFTowerBase*, OldControlledTower);

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFPlayerController : public APlayerController, public IPlayerTowerControlInterface
{
	GENERATED_BODY()
	
public:
	ADFPlayerController();

	FORCEINLINE class ADFStructureBase* GetStructureUnderCursor() { return CurrentStructureUnderCursor.Get(); }

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginCursorOverStructureCallback(AActor* TouchedActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnEndCursorOverStructureCallback(AActor* TouchedActor);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void StartTowerControl_Implementation(class ADFTowerBase* NewTower) override;
	virtual void EndTowerControl_Implementation() override;
	virtual class ADFTowerBase* GetCurrentControlledTower_Implementation() override { return CurrentControlledTower.Get(); }

	UFUNCTION()
	void OnRep_CurrentControlledTower();

public:
	UPROPERTY(BlueprintAssignable)
	FOnTowerControlStartDelegate OnTowerControlStart;

	UPROPERTY(BlueprintAssignable)
	FOnTowerControlEndDelegate OnTowerControlEnd;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<class ADFPlayerPawn> DFPlayerPawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Control")
	TWeakObjectPtr<class ADFStructureBase> CurrentStructureUnderCursor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentControlledTower, Category = "Control")
	TWeakObjectPtr<class ADFTowerBase> CurrentControlledTower;
};
