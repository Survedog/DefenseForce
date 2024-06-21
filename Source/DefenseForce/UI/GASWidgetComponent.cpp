// Copyright 2024, Survedog. All rights reserved.


#include "UI/GASWidgetComponent.h"
#include "UI/GASUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGASWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UGASUserWidget* GASWidget = Cast<UGASUserWidget>(GetWidget());
	if (GASWidget)
	{
		GASWidget->SetAbilitySystemComponent(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
	}
}
