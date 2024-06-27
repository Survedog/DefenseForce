// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DFGaugeWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DEFENSEFORCE_API UDFGaugeWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void UpdateGaugeCurrentValue(float InCurrentValue) { GaugeCurrentValue = InCurrentValue; UpdateGauge(GaugeCurrentValue, GaugeMaxValue); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void UpdateGaugeMaxValue(float InMaxValue) { GaugeMaxValue = InMaxValue; UpdateGauge(GaugeCurrentValue, GaugeMaxValue); }

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGauge(float InCurrentValue, float InMaxValue);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetGaugeCurrentValue() const { return GaugeCurrentValue; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetGaugeMaxValue() const { return GaugeMaxValue; }

private:
	UPROPERTY(VisibleAnywhere)
	float GaugeCurrentValue;

	UPROPERTY(VisibleAnywhere)
	float GaugeMaxValue;
};
