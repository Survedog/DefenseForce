// Copyright 2024, Survedog. All rights reserved.


#include "Character/DFEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/DFCharacterAttributeSet.h"
#include "GAS/Attribute/DFHealthAttributeSet.h"
#include "DFLog.h"

ADFEnemyCharacter::ADFEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	DropMoneyAmount = 100.0f;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	CharacterAttributeSet = CreateDefaultSubobject<UDFCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
	HealthAttributeSet = CreateDefaultSubobject<UDFHealthAttributeSet>(TEXT("HealthAttributeSet"));

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

#pragma region AttributeGetterSetters
float ADFEnemyCharacter::GetHp() const
{
	return HealthAttributeSet->GetHp();
}

void ADFEnemyCharacter::SetHp(float NewHp)
{
	HealthAttributeSet->SetHp(NewHp);
}

float ADFEnemyCharacter::GetMaxHp() const
{
	return HealthAttributeSet->GetMaxHp();
}

void ADFEnemyCharacter::SetMaxHp(float NewMaxHp)
{
	HealthAttributeSet->SetMaxHp(NewMaxHp);
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
	
	DF_NETLOG(LogDF, Log, TEXT("Start"));
	ASC->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		HealthAttributeSet->OnHpZero.AddDynamic(this, &ADFEnemyCharacter::OnDead);

		for (auto Ability : InnateAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void ADFEnemyCharacter::OnDead_Implementation()
{
	DF_NETLOG(LogDF, Log, TEXT("Start"));	
}
