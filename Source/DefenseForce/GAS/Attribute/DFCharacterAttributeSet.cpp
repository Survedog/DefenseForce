// Copyright 2024, Survedog. All rights reserved.


#include "GAS/Attribute/DFCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

UDFCharacterAttributeSet::UDFCharacterAttributeSet() : MaxHp(100.0f), DamageToApply(0.0f)
{
	InitHp(MaxHp.GetBaseValue());
}

void UDFCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// Don't call original super function.
	if (Attribute == GetHpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHp());
	}
}

void UDFCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	// Don't call original super function.
	if (Attribute == GetHpAttribute())
	{
		if (GetOwningActor()->HasAuthority() && FMath::IsNearlyZero(NewValue) && !bIsDead)
		{
			bIsDead = true;
			OnHpZero.Broadcast();
		}
	}
}

void UDFCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDFCharacterAttributeSet, Hp, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UDFCharacterAttributeSet, MaxHp, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UDFCharacterAttributeSet, MoveSpeed, COND_None, REPNOTIFY_OnChanged);
}

void UDFCharacterAttributeSet::OnRep_Hp(const FGameplayAttributeData& OldHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDFCharacterAttributeSet, Hp, OldHp);
}

void UDFCharacterAttributeSet::OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDFCharacterAttributeSet, MaxHp, OldMaxHp);
}

void UDFCharacterAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDFCharacterAttributeSet, MoveSpeed, OldMoveSpeed);
}
