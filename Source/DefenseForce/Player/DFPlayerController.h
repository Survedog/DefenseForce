// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DFPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlStartDelegate, ADFTowerBase*, NewControlledTower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlEndDelegate, ADFTowerBase*, OldControlledTower);

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFPlayerController : public APlayerController
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
	friend class GA_TowerControl;

	UFUNCTION(BlueprintCallable)
	void StartTowerControl(class ADFTowerBase* NewTower);

	UFUNCTION(BlueprintCallable)
	void EndTowerControl();

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
