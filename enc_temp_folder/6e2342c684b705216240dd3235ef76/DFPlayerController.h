// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerTowerControlInterface.h"
#include "Interface/PlayerBuildModeInterface.h"
#include "DFPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlStartDelegate, ADFTowerBase*, NewControlledTower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerControlEndDelegate, ADFTowerBase*, OldControlledTower);

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFPlayerController : public APlayerController, public IPlayerTowerControlInterface, public IPlayerBuildModeInterface
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
	/* PlayerTowerControlInterface */
	virtual void StartTowerControl(class ADFTowerBase* NewTower) override;
	virtual void EndTowerControl() override;
	virtual class ADFTowerBase* GetCurrentControlledTower() const override { return CurrentControlledTower.Get(); }
	virtual class ADFStructureBase* GetCurrentStructureUnderCursor() const override { return CurrentStructureUnderCursor.Get(); }

	UFUNCTION()
	void OnRep_CurrentControlledTower();

protected:
	/* PlayerBuildModeInterface */
	virtual void EnterBuildMode(TSubclassOf<class ADFStructureBase> InTargetStructureClass) override;
	virtual void ExitBuildMode(class ADFStructureBase* InBuiltStructure) override;
	
	bool CanBuildStructure(TSubclassOf<class ADFStructureBase> InTargetStructureClass) const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnterBuildMode(TSubclassOf<class ADFStructureBase> InTargetStructureClass);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExitBuildMode(class ADFStructureBase* InBuiltStructure);

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
	TObjectPtr<class ADFTowerBase> CurrentControlledTower;
};
