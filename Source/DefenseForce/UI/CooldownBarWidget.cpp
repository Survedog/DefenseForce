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
	}

	if (ActiveCooldownEffect)
	{
		const float CooldownDuration = ActiveCooldownEffect->GetDuration();
		const float CooldownElaspedTime = CooldownDuration - ActiveCooldownEffect->GetTimeRemaining(GetWorld()->GetTimeSeconds());

		if (CooldownDuration > 0.0f)
		{
			const float CooldownElapsedPercent = FMath::Clamp(CooldownElaspedTime / CooldownDuration, 0.0f, 1.0f);
			PbCooldownBar->SetPercent(CooldownElapsedPercent);
		}
	}
}

void UCooldownBarWidget::OnCooldownTagChanged(const FGameplayTag Tag, int32 TagCount)
{
	DF_NETUILOG(LogDFUI, Log, TEXT("Start. TagCount: %d"), TagCount);
	ActiveCooldownEffect = nullptr;
	if (TagCount > 0)
	{
		PbCooldownBar->SetPercent(0.0f);
		SetVisibility(ESlateVisibility::HitTestInvisible);		
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
