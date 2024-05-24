// Copyright 2024, Survedog. All rights reserved.


#include "Character/DFEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/DFCharacterAttributeSet.h"

ADFEnemyCharacter::ADFEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	CharacterAttributeSet = CreateDefaultSubobject<UDFCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	static ConstructorHelpers::FClassFinder<AController> AIControllerClassRef(TEXT("/Game/DefenseForce/Blueprint/Character/Enemy/BP_DFAIController.BP_DFAIController_C"));
	if (AIControllerClassRef.Class)
	{
		AIControllerClass = AIControllerClassRef.Class;
	}
}

UAbilitySystemComponent* ADFEnemyCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

#pragma region AttributeGetterSetter
float ADFEnemyCharacter::GetHp() const
{
	return CharacterAttributeSet->GetHp();
}

void ADFEnemyCharacter::SetHp(float NewHp)
{
	CharacterAttributeSet->SetHp(NewHp);
}

float ADFEnemyCharacter::GetMaxHp() const
{
	return CharacterAttributeSet->GetMaxHp();
}

void ADFEnemyCharacter::SetMaxHp(float NewMaxHp)
{
	CharacterAttributeSet->SetMaxHp(NewMaxHp);
}

float ADFEnemyCharacter::GetMoveSpeed() const
{
	return CharacterAttributeSet->GetMoveSpeed();
}

void ADFEnemyCharacter::SetMoveSpeed(float NewMoveSpeed)
{
	CharacterAttributeSet->SetMoveSpeed(NewMoveSpeed);
}
#pragma endregion

void ADFEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ASC->InitAbilityActorInfo(this, this);
}
