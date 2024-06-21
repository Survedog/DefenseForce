// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "GASUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UGASUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	virtual void SetAbilitySystemComponent(UAbilitySystemComponent* NewASC) { ASC = NewASC; }

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC.Get(); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TWeakObjectPtr<class UAbilitySystemComponent> ASC;
};
