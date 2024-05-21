// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADFPlayerController();

	UFUNCTION(BlueprintCallable)
	void StartTowerControl(class ADFTowerBase* NewTower);

	UFUNCTION(BlueprintCallable)
	void EndTowerControl();

protected:
	virtual void OnPossess(APawn* aPawn) override;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player")
	TObjectPtr<class ADFPlayerPawn> DFPlayerPawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Control")
	TWeakObjectPtr<class ADFTowerBase> CurrentControlledTower;
};
