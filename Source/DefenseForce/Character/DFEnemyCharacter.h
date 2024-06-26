// Copyright 2024, Survedog. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/DFCharacterAnimAttackInterface.h"
#include "DFEnemyCharacter.generated.h"

UCLASS(Abstract)
class DEFENSEFORCE_API ADFEnemyCharacter : public ACharacter, public IAbilitySystemInterface, public IDFCharacterAnimAttackInterface
{
	GENERATED_BODY()

public:
	ADFEnemyCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void AttackHitCheck_Implementation() override;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnDead();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AActor* GetAttackTargetActor() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	FORCEINLINE float GetDamageAmount() const;

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

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UDFCharacterAttributeSet> CharacterAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UDFHealthAttributeSet> HealthAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> InnateAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;
};
