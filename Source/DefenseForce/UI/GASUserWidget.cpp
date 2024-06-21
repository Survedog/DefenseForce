// Copyright 2024, Survedog. All rights reserved.


#include "UI/GASUserWidget.h"
#include "AbilitySystemComponent.h"

void UGASUserWidget::SetAbilitySystemComponent(UAbilitySystemComponent* NewASC)
{
	ASC = NewASC;
}

UAbilitySystemComponent* UGASUserWidget::GetAbilitySystemComponent() const
{
	return ASC.Get();
}
