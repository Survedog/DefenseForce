// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Structure/DFStructureBase.h"
#include "DFTowerBase.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API ADFTowerBase : public ADFStructureBase
{
	GENERATED_BODY()
	
public:
	ADFTowerBase();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsBeingControlled() { return bIsBeingControlled; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginControl(class ADFPlayerPawn* NewControllingPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndControl();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_ControllingPlayerPawn();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_ControllingPlayerPawn, Category = "Player Control")
	TWeakObjectPtr<class ADFPlayerPawn> ControllingPlayerPawn;

private:
	UPROPERTY(VisibleInstanceOnly)
	uint8 bIsBeingControlled : 1;
};
