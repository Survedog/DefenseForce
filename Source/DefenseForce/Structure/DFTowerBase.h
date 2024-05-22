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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsLocallyControlled() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnControlStart(class ADFPlayerPawn* NewPlayerPawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnControlEnd();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_ControllingPlayerPawn();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_ControllingPlayerPawn, Category = "Player Control")
	TWeakObjectPtr<class ADFPlayerPawn> ControllingPlayerPawn;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Player Control")
	uint8 bIsBeingControlled : 1;
};
