// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DFCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpZeroDelegate);

/**
 * 
 */
UCLASS()
class DEFENSEFORCE_API UDFCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UDFCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UDFCharacterAttributeSet, Hp);
	ATTRIBUTE_ACCESSORS(UDFCharacterAttributeSet, MaxHp);
	ATTRIBUTE_ACCESSORS(UDFCharacterAttributeSet, DamageToApply);
	ATTRIBUTE_ACCESSORS(UDFCharacterAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintAssignable, Meta = (HideInDetailsView))
	mutable FOnHpZeroDelegate OnHpZero;

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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

protected:
	uint8 bIsDead : 1;

protected:
	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& OldHp);

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& OldMaxHp);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
};
