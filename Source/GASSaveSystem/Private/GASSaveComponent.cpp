#include "GASSaveComponent.h"
#include "GASSaveSystemLibrary.h"
#include "GameFramework/Actor.h"

UGASSaveComponent::UGASSaveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoLoadOnBeginPlay = true;
	DefaultSaveSlot = TEXT("GASSaveSlot");
	SavedActorID = TEXT("Player_0");
}

void UGASSaveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SavedActorID.IsEmpty() && GetOwner())
	{
		SavedActorID = GetOwner()->GetName();
	}

	if (bAutoLoadOnBeginPlay)
	{
		LoadGASState(DefaultSaveSlot);
	}
}

bool UGASSaveComponent::SaveGASState(const FString& SlotName)
{
	if (!GetOwner()) return false;

	const FString TargetSlot = SlotName.IsEmpty() ? DefaultSaveSlot : SlotName;
	return UGASSaveSystemLibrary::SaveActorGASToSlot(this, GetOwner(), SavedActorID, TargetSlot);
}

bool UGASSaveComponent::LoadGASState(const FString& SlotName)
{
	if (!GetOwner()) return false;

	const FString TargetSlot = SlotName.IsEmpty() ? DefaultSaveSlot : SlotName;
	return UGASSaveSystemLibrary::LoadActorGASFromSlot(this, GetOwner(), SavedActorID, TargetSlot);
}
