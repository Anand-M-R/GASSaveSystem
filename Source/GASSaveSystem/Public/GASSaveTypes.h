#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "GASSaveTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;

/** Stores serialized data for a single Attribute (e.g. Health, Mana, XP) */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASAttributeSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	FName AttributeName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	float BaseValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	float CurrentValue = 0.0f;
};

/** Stores serialized data for a single Granted Ability */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASAbilitySaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	int32 AbilityLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	int32 InputID = -1;
};

/** Stores serialized data for an Active Gameplay Effect (Buff/Debuff) */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASActiveEffectSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TSubclassOf<UGameplayEffect> EffectClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	float Duration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	float TimeRemaining = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	int32 StackCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	int32 EffectLevel = 1;
};

/** Complete container of all saved GAS data for a single Actor */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	FString SavedActorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TArray<FGASAttributeSaveData> SavedAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	FGameplayTagContainer SavedGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TArray<FGASAbilitySaveData> SavedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TArray<FGASActiveEffectSaveData> SavedActiveEffects;
};

/** Options to selectively save specific elements of the Ability System Component */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASSaveOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Options")
	bool bSaveAttributes = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Options")
	bool bSaveGameplayTags = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Options")
	bool bSaveGrantedAbilities = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Options")
	bool bSaveActiveEffects = true;
};

/** Options to selectively restore specific elements onto the Ability System Component */
USTRUCT(BlueprintType)
struct GASSAVESYSTEM_API FGASRestoreOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Restore Options")
	bool bRestoreAttributes = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Restore Options")
	bool bRestoreGameplayTags = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Restore Options")
	bool bRestoreGrantedAbilities = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Restore Options")
	bool bRestoreActiveEffects = true;
};
