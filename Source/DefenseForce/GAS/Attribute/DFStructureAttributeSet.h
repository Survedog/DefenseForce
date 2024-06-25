// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DFStructureAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestructedDelegate);

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UDFStructureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UDFStructureAttributeSet();

	ATTRIBUTE_ACCESSORS(UDFStructureAttributeSet, Hp);
	ATTRIBUTE_ACCESSORS(UDFStructureAttributeSet, MaxHp);
	ATTRIBUTE_ACCESSORS(UDFStructureAttributeSet, DamageToApply);

	UPROPERTY(BlueprintAssignable, Meta = (HideInDetailsView))
	mutable FOnDestructedDelegate OnDestructed;

protected:
	//virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	//virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Hp, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Hp;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHp, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHp;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageToApply;

protected:
	uint8 bIsDestructed : 1;

protected:
	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& OldHp);

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp);
};
