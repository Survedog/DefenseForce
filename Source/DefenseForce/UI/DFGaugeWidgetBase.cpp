// Copyright 2024, Survedog. All rights reserved.


#include "UI/DFGaugeWidgetBase.h"

UDFGaugeWidgetBase::UDFGaugeWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GaugeCurrentValue = 0.0f;
	GaugeMaxValue = 1.0f;
}
