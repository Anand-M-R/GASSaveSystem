#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GASSaveTypes.h"
#include "GASSaveGame.generated.h"

/**
 * USaveGame class used by GASSaveSystem to store multiple actor GAS states on disk.
 */
UCLASS(BlueprintType)
class GASSAVESYSTEM_API UGASSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGASSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	int32 UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	FDateTime SaveTimestamp;

	/** Map of Actor Saved ID -> Serialized GAS state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS Save System")
	TMap<FString, FGASActorSaveData> SavedGASActors;
};
