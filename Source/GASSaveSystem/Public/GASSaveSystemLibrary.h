#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASSaveTypes.h"
#include "GASSaveSystemLibrary.generated.h"

class UAbilitySystemComponent;
class UGASSaveGame;

/**
 * Blueprint & C++ Function Library for saving and restoring Gameplay Ability System components.
 */
UCLASS()
class GASSAVESYSTEM_API UGASSaveSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Extract attributes, tags, granted abilities, and active effects from an AbilitySystemComponent.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System")
	static bool SaveAbilitySystemComponent(UAbilitySystemComponent* ASC, FGASActorSaveData& OutSaveData);

	/**
	 * Restore attributes, tags, granted abilities, and active effects onto an AbilitySystemComponent.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System")
	static bool RestoreAbilitySystemComponent(UAbilitySystemComponent* ASC, const FGASActorSaveData& InSaveData);

	/**
	 * Saves a single Actor's GAS state directly to a specified Save Game slot on disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool SaveActorGASToSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);

	/**
	 * Loads a single Actor's GAS state from a Save Game slot on disk and restores it onto the target Actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool LoadActorGASFromSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);
};
