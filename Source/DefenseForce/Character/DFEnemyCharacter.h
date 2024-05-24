// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DFEnemyCharacter.generated.h"

UCLASS()
class DEFENSEFORCE_API ADFEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADFEnemyCharacter();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UDFCharacterAttributeSet> CharacterAttributeSet;
};
