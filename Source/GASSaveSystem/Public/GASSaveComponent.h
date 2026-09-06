#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GASSaveTypes.h"
#include "GASSaveComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGASSaveEvent, const FGASActorSaveData&, SaveData);

/**
 * Drop-in ActorComponent that automates GAS saving/loading for any Actor.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASSAVESYSTEM_API UGASSaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGASSaveComponent();

	/** Unique identifier for this actor in save files */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Component")
	FString SavedActorID;

	/** Auto-load GAS state on BeginPlay if save file exists */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Component")
	bool bAutoLoadOnBeginPlay;

	/** Slot name to save to/load from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Component")
	FString DefaultSaveSlot;

	/** Filtering options to choose which elements to save (Attributes, Tags, Abilities, Effects) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Component")
	FGASSaveOptions SaveOptions;

	/** Filtering options to choose which elements to restore (Attributes, Tags, Abilities, Effects) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save Component")
	FGASRestoreOptions RestoreOptions;

	UPROPERTY(BlueprintAssignable, Category = "GAS Save Component")
	FOnGASSaveEvent OnBeforeGASSaved;

	UPROPERTY(BlueprintAssignable, Category = "GAS Save Component")
	FOnGASSaveEvent OnAfterGASLoaded;

	/** Save this actor's GAS state */
	UFUNCTION(BlueprintCallable, Category = "GAS Save Component")
	bool SaveGASState(const FString& SlotName = TEXT("GASSaveSlot"));

	/** Load this actor's GAS state */
	UFUNCTION(BlueprintCallable, Category = "GAS Save Component")
	bool LoadGASState(const FString& SlotName = TEXT("GASSaveSlot"));

	/** Get raw saved GAS data for this actor from disk */
	UFUNCTION(BlueprintCallable, Category = "GAS Save Component")
	bool GetSavedGASData(FGASActorSaveData& OutSaveData, const FString& SlotName = TEXT("GASSaveSlot"));

	/** Clear saved GAS data for this actor from disk */
	UFUNCTION(BlueprintCallable, Category = "GAS Save Component")
	bool ClearSavedGASData(const FString& SlotName = TEXT("GASSaveSlot"));

protected:
	virtual void BeginPlay() override;
};
