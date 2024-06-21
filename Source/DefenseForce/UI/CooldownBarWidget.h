// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/GASUserWidget.h"
#include "GameplayTagContainer.h"
#include "CooldownBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UCooldownBarWidget : public UGASUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OnCooldownTagChanged(const FGameplayTag Tag, int32 TagCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PbCooldownBar;

	const struct FActiveGameplayEffect* ActiveCooldownEffect;
};
