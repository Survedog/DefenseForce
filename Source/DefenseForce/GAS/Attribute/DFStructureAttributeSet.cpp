// Copyright 2024, Survedog. All rights reserved.


#include "GAS/Attribute/DFStructureAttributeSet.h"
#include "Net/UnrealNetwork.h"

UDFStructureAttributeSet::UDFStructureAttributeSet() : MaxHp(100.0f)
{
	InitHp(MaxHp.GetBaseValue());
}

void UDFStructureAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// Don't call original super function.
	if (Attribute == GetHpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHp());
	}
}

void UDFStructureAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	// Don't call original super function.
	if (Attribute == GetHpAttribute())
	{
		if (GetOwningActor()->HasAuthority() && FMath::IsNearlyZero(NewValue) && !bIsDestructed)
		{
			bIsDestructed = true;
			OnDestructed.Broadcast();
		}
	}
}

void UDFStructureAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDFStructureAttributeSet, Hp, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UDFStructureAttributeSet, MaxHp, COND_None, REPNOTIFY_OnChanged);
}

void UDFStructureAttributeSet::OnRep_Hp(const FGameplayAttributeData& OldHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDFStructureAttributeSet, Hp, OldHp);
}

void UDFStructureAttributeSet::OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDFStructureAttributeSet, MaxHp, OldMaxHp);
}
