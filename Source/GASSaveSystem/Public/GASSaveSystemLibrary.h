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
	 * Extract attributes, tags, granted abilities, and active effects from an AbilitySystemComponent with filter options.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System")
	static bool SaveAbilitySystemComponent(UAbilitySystemComponent* ASC, FGASActorSaveData& OutSaveData, FGASSaveOptions SaveOptions = FGASSaveOptions());

	/**
	 * Restore attributes, tags, granted abilities, and active effects onto an AbilitySystemComponent with filter options.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System")
	static bool RestoreAbilitySystemComponent(UAbilitySystemComponent* ASC, const FGASActorSaveData& InSaveData, FGASRestoreOptions RestoreOptions = FGASRestoreOptions());

	/**
	 * Saves a single Actor's GAS state directly to a specified Save Game slot on disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool SaveActorGASToSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0, FGASSaveOptions SaveOptions = FGASSaveOptions());

	/**
	 * Loads a single Actor's GAS state from a Save Game slot on disk and restores it onto the target Actor.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool LoadActorGASFromSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0, FGASRestoreOptions RestoreOptions = FGASRestoreOptions());

	/**
	 * Save multiple Actors' GAS states into a single Save Game slot on disk at once.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool SaveMultipleActorsGASToSlot(UObject* WorldContextObject, const TArray<AActor*>& TargetActors, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0, FGASSaveOptions SaveOptions = FGASSaveOptions());

	/**
	 * Load multiple Actors' GAS states from a single Save Game slot on disk at once.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool LoadMultipleActorsGASFromSlot(UObject* WorldContextObject, const TArray<AActor*>& TargetActors, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0, FGASRestoreOptions RestoreOptions = FGASRestoreOptions());

	/**
	 * Check if a GAS Save Game slot file exists on disk.
	 */
	UFUNCTION(BlueprintPure, Category = "GAS Save System")
	static bool DoesGASSaveSlotExist(const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);

	/**
	 * Delete a GAS Save Game slot file from disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System")
	static bool DeleteGASSaveSlot(const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);

	/**
	 * Read an Actor's raw saved GAS data structure from a Save Game slot without restoring it immediately.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool GetSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, FGASActorSaveData& OutSaveData, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);

	/**
	 * Write custom GAS save data for a specific SavedActorID into a Save Game slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool SetSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, const FGASActorSaveData& InSaveData, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);

	/**
	 * Remove a specific Actor's saved GAS data from a Save Game slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS Save System", meta = (WorldContext = "WorldContextObject"))
	static bool ClearSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, const FString& SlotName = TEXT("GASSaveSlot"), int32 UserIndex = 0);
};
