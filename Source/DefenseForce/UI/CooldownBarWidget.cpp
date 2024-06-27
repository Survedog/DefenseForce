// Copyright 2024, Survedog. All rights reserved.


#include "UI/CooldownBarWidget.h"
#include "AbilitySystemComponent.h"
#include "GAS/DFGameplayTags.h"
#include "Components/ProgressBar.h"
#include "DFLog.h"

void UCooldownBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	DF_NETUILOG(LogDFUI, Log, TEXT("Start"));
	if (ASC.IsValid())
	{
		ASC->RegisterGameplayTagEvent(GASTAG_Structure_Action_Attack_Cooldown, EGameplayTagEventType::Type::NewOrRemoved).AddUObject(this, &UCooldownBarWidget::OnCooldownTagChanged);
	}	
	SetVisibility(ESlateVisibility::Hidden);
}

void UCooldownBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ActiveCooldownEffect)
	{
		const TArray<FActiveGameplayEffectHandle>& EffectHandleArray = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(GASTAG_Structure_Action_Attack_Cooldown));
		if (!EffectHandleArray.IsEmpty())
		{
			ActiveCooldownEffect = ASC->GetActiveGameplayEffect(EffectHandleArray[0]);
		}
		const float CooldownDuration = ActiveCooldownEffect->GetDuration();
		UpdateGaugeMaxValue(CooldownDuration);
	}

	if (ActiveCooldownEffect)
	{
		const float CooldownElaspedTime = ActiveCooldownEffect->GetDuration() - ActiveCooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
		UpdateGaugeCurrentValue(CooldownElaspedTime);
	}
}

void UCooldownBarWidget::OnCooldownTagChanged(const FGameplayTag Tag, int32 TagCount)
{
	DF_NETUILOG(LogDFUI, Log, TEXT("Start. TagCount: %d"), TagCount);
	ActiveCooldownEffect = nullptr;
	if (TagCount > 0)
	{
		UpdateGaugeUI(GetGaugeCurrentValue(), GetGaugeMaxValue());
		SetVisibility(ESlateVisibility::HitTestInvisible);		
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCooldownBarWidget::SetAbilitySystemComponent(UAbilitySystemComponent* NewASC)
{
	ASC = NewASC;
}

UAbilitySystemComponent* UCooldownBarWidget::GetAbilitySystemComponent() const
{
	return ASC.Get();
}
