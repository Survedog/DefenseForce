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
	FORCEINLINE bool IsBeingControlled() const { return bIsBeingControlled; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginControl(class ADFPlayerController* NewControllingPlayer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndControl();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(BlueprintNativeEvent)
	//void OnRep_ControllingPlayer();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, /*ReplicatedUsing = OnRep_ControllingPlayer,*/ Category = "Player Control")
	TWeakObjectPtr<class ADFPlayerController> ControllingPlayer;

private:
	UPROPERTY(VisibleInstanceOnly)
	uint8 bIsBeingControlled : 1;
};
