// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/DFAIAttackInterface.h"
#include "DFEnemyCharacter.generated.h"

UCLASS(Abstract)
class DEFENSEFORCE_API ADFEnemyCharacter : public ACharacter, public IAbilitySystemInterface, public IDFAIAttackInterface
{
	GENERATED_BODY()

public:
	ADFEnemyCharacter();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual EBTNodeResult::Type Attack_Implementation(AActor* InAttackTarget) override PURE_VIRTUAL(ADFEnemyCharacter::Attack, return EBTNodeResult::Type::Succeeded;);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnDead();

// General
public:
	float GetDropMoneyAmount() const { return DropMoneyAmount; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float DropMoneyAmount;

//Attributes
public:
	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE float GetHp() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE void SetHp(float NewHp);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE float GetMaxHp() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE void SetMaxHp(float NewMaxHp);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE float GetMoveSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE void SetMoveSpeed(float NewMoveSpeed);

	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE float GetDamageToApply() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
	FORCEINLINE void SetDamageToApply(float NewDamageToApply);

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UDFCharacterAttributeSet> CharacterAttributeSet;
};
