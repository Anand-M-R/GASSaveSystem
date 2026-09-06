#include "GASSaveSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GASSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpec.h"
#include "Engine/World.h"

bool UGASSaveSystemLibrary::SaveAbilitySystemComponent(UAbilitySystemComponent* ASC, FGASActorSaveData& OutSaveData, FGASSaveOptions SaveOptions)
{
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGASSaveSystemLibrary::SaveAbilitySystemComponent - ASC is null!"));
		return false;
	}

	OutSaveData.SavedAttributes.Empty();
	OutSaveData.SavedAbilities.Empty();
	OutSaveData.SavedActiveEffects.Empty();
	OutSaveData.SavedGameplayTags.Reset();

	// 1. Save Attributes
	if (SaveOptions.bSaveAttributes)
	{
		const TArray<UAttributeSet*>& AttributeSets = ASC->GetSpawnedAttributes();
		for (UAttributeSet* Set : AttributeSets)
		{
			if (!Set) continue;

			for (TFieldIterator<FProperty> It(Set->GetClass()); It; ++It)
			{
				FStructProperty* StructProp = CastField<FStructProperty>(*It);
				if (StructProp && StructProp->Struct == FGameplayAttributeData::StaticStruct())
				{
					FGameplayAttribute Attribute(StructProp);
					if (Attribute.IsValid() && ASC->HasAttributeSetForAttribute(Attribute))
					{
						FGASAttributeSaveData AttrData;
						AttrData.AttributeName = FName(*Attribute.GetName());
						AttrData.BaseValue = ASC->GetNumericAttributeBase(Attribute);
						AttrData.CurrentValue = ASC->GetNumericAttribute(Attribute);

						OutSaveData.SavedAttributes.Add(AttrData);
					}
				}
			}
		}
	}

	// 2. Save Gameplay Tags
	if (SaveOptions.bSaveGameplayTags)
	{
		ASC->GetOwnedGameplayTags(OutSaveData.SavedGameplayTags);
	}

	// 3. Save Granted Abilities
	if (SaveOptions.bSaveGrantedAbilities)
	{
		const TArray<FGameplayAbilitySpec>& ActivatableAbilities = ASC->GetActivatableAbilities();
		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities)
		{
			if (Spec.Ability)
			{
				FGASAbilitySaveData AbilityData;
				AbilityData.AbilityClass = Spec.Ability->GetClass();
				AbilityData.AbilityLevel = Spec.Level;
				AbilityData.InputID = Spec.InputID;

				OutSaveData.SavedAbilities.Add(AbilityData);
			}
		}
	}

	// 4. Save Active Gameplay Effects
	if (SaveOptions.bSaveActiveEffects)
	{
		FGameplayEffectQuery Query;
		TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = ASC->GetActiveEffects(Query);
		for (const FActiveGameplayEffectHandle& Handle : ActiveEffectHandles)
		{
			const FActiveGameplayEffect* ActiveEffect = ASC->GetActiveGameplayEffect(Handle);
			if (ActiveEffect && ActiveEffect->Spec.Def)
			{
				FGASActiveEffectSaveData EffectData;
				EffectData.EffectClass = ActiveEffect->Spec.Def->GetClass();
				EffectData.Duration = ActiveEffect->GetDuration();
				EffectData.TimeRemaining = ActiveEffect->GetTimeRemaining(ASC->GetWorld() ? ASC->GetWorld()->GetTimeSeconds() : 0.0f);
				EffectData.StackCount = ActiveEffect->Spec.GetStackCount();
				EffectData.EffectLevel = ActiveEffect->Spec.GetLevel();

				OutSaveData.SavedActiveEffects.Add(EffectData);
			}
		}
	}

	return true;
}

bool UGASSaveSystemLibrary::RestoreAbilitySystemComponent(UAbilitySystemComponent* ASC, const FGASActorSaveData& InSaveData, FGASRestoreOptions RestoreOptions)
{
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGASSaveSystemLibrary::RestoreAbilitySystemComponent - ASC is null!"));
		return false;
	}

	// 1. Restore Attributes
	if (RestoreOptions.bRestoreAttributes)
	{
		const TArray<UAttributeSet*>& AttributeSets = ASC->GetSpawnedAttributes();
		for (const FGASAttributeSaveData& AttrData : InSaveData.SavedAttributes)
		{
			for (UAttributeSet* Set : AttributeSets)
			{
				if (!Set) continue;

				for (TFieldIterator<FProperty> It(Set->GetClass()); It; ++It)
				{
					FStructProperty* StructProp = CastField<FStructProperty>(*It);
					if (StructProp && StructProp->Struct == FGameplayAttributeData::StaticStruct())
					{
						FGameplayAttribute Attribute(StructProp);
						if (Attribute.IsValid() && FName(*Attribute.GetName()) == AttrData.AttributeName)
						{
							ASC->SetNumericAttributeBase(Attribute, AttrData.BaseValue);
							ASC->ApplyModToAttribute(Attribute, EGameplayModOp::Override, AttrData.CurrentValue);
						}
					}
				}
			}
		}
	}

	// 2. Restore Granted Abilities
	if (RestoreOptions.bRestoreGrantedAbilities)
	{
		for (const FGASAbilitySaveData& AbilityData : InSaveData.SavedAbilities)
		{
			if (AbilityData.AbilityClass)
			{
				if (!ASC->FindAbilitySpecFromClass(AbilityData.AbilityClass))
				{
					FGameplayAbilitySpec Spec(AbilityData.AbilityClass, AbilityData.AbilityLevel, AbilityData.InputID);
					ASC->GiveAbility(Spec);
				}
			}
		}
	}

	// 3. Restore Active Gameplay Effects
	if (RestoreOptions.bRestoreActiveEffects)
	{
		for (const FGASActiveEffectSaveData& EffectData : InSaveData.SavedActiveEffects)
		{
			if (EffectData.EffectClass)
			{
				FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectData.EffectClass, EffectData.EffectLevel, Context);
				if (SpecHandle.IsValid())
				{
					SpecHandle.Data->SetStackCount(EffectData.StackCount);
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}

	// 4. Restore Gameplay Tags
	if (RestoreOptions.bRestoreGameplayTags && InSaveData.SavedGameplayTags.Num() > 0)
	{
		ASC->AddLooseGameplayTags(InSaveData.SavedGameplayTags);
	}

	return true;
}

bool UGASSaveSystemLibrary::SaveActorGASToSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName, int32 UserIndex, FGASSaveOptions SaveOptions)
{
	if (!TargetActor) return false;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (!ASC) return false;

	UGASSaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::CreateSaveGameObject(UGASSaveGame::StaticClass()));
		SaveGameObject->SaveSlotName = SlotName;
		SaveGameObject->UserIndex = UserIndex;
	}

	FGASActorSaveData SaveData;
	SaveData.SavedActorID = SavedActorID;
	if (SaveAbilitySystemComponent(ASC, SaveData, SaveOptions))
	{
		SaveGameObject->SavedGASActors.Add(SavedActorID, SaveData);
		SaveGameObject->SaveTimestamp = FDateTime::Now();
		return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex);
	}

	return false;
}

bool UGASSaveSystemLibrary::LoadActorGASFromSlot(UObject* WorldContextObject, AActor* TargetActor, const FString& SavedActorID, const FString& SlotName, int32 UserIndex, FGASRestoreOptions RestoreOptions)
{
	if (!TargetActor) return false;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (!ASC) return false;

	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) return false;

	UGASSaveGame* SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!SaveGameObject) return false;

	if (const FGASActorSaveData* FoundData = SaveGameObject->SavedGASActors.Find(SavedActorID))
	{
		return RestoreAbilitySystemComponent(ASC, *FoundData, RestoreOptions);
	}

	return false;
}

bool UGASSaveSystemLibrary::SaveMultipleActorsGASToSlot(UObject* WorldContextObject, const TArray<AActor*>& TargetActors, const FString& SlotName, int32 UserIndex, FGASSaveOptions SaveOptions)
{
	if (TargetActors.Num() == 0) return false;

	UGASSaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::CreateSaveGameObject(UGASSaveGame::StaticClass()));
		SaveGameObject->SaveSlotName = SlotName;
		SaveGameObject->UserIndex = UserIndex;
	}

	int32 SavedCount = 0;
	for (AActor* Actor : TargetActors)
	{
		if (!Actor) continue;

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		if (!ASC) continue;

		FString ActorID = Actor->GetName();
		FGASActorSaveData SaveData;
		SaveData.SavedActorID = ActorID;

		if (SaveAbilitySystemComponent(ASC, SaveData, SaveOptions))
		{
			SaveGameObject->SavedGASActors.Add(ActorID, SaveData);
			SavedCount++;
		}
	}

	if (SavedCount > 0)
	{
		SaveGameObject->SaveTimestamp = FDateTime::Now();
		return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex);
	}

	return false;
}

bool UGASSaveSystemLibrary::LoadMultipleActorsGASFromSlot(UObject* WorldContextObject, const TArray<AActor*>& TargetActors, const FString& SlotName, int32 UserIndex, FGASRestoreOptions RestoreOptions)
{
	if (TargetActors.Num() == 0) return false;
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) return false;

	UGASSaveGame* SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!SaveGameObject) return false;

	int32 RestoredCount = 0;
	for (AActor* Actor : TargetActors)
	{
		if (!Actor) continue;

		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
		if (!ASC) continue;

		FString ActorID = Actor->GetName();
		if (const FGASActorSaveData* FoundData = SaveGameObject->SavedGASActors.Find(ActorID))
		{
			if (RestoreAbilitySystemComponent(ASC, *FoundData, RestoreOptions))
			{
				RestoredCount++;
			}
		}
	}

	return RestoredCount > 0;
}

bool UGASSaveSystemLibrary::DoesGASSaveSlotExist(const FString& SlotName, int32 UserIndex)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UGASSaveSystemLibrary::DeleteGASSaveSlot(const FString& SlotName, int32 UserIndex)
{
	return UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
}

bool UGASSaveSystemLibrary::GetSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, FGASActorSaveData& OutSaveData, const FString& SlotName, int32 UserIndex)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) return false;

	UGASSaveGame* SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!SaveGameObject) return false;

	if (const FGASActorSaveData* FoundData = SaveGameObject->SavedGASActors.Find(SavedActorID))
	{
		OutSaveData = *FoundData;
		return true;
	}

	return false;
}

bool UGASSaveSystemLibrary::SetSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, const FGASActorSaveData& InSaveData, const FString& SlotName, int32 UserIndex)
{
	UGASSaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::CreateSaveGameObject(UGASSaveGame::StaticClass()));
		SaveGameObject->SaveSlotName = SlotName;
		SaveGameObject->UserIndex = UserIndex;
	}

	SaveGameObject->SavedGASActors.Add(SavedActorID, InSaveData);
	SaveGameObject->SaveTimestamp = FDateTime::Now();
	return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex);
}

bool UGASSaveSystemLibrary::ClearSavedActorGASData(UObject* WorldContextObject, const FString& SavedActorID, const FString& SlotName, int32 UserIndex)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex)) return false;

	UGASSaveGame* SaveGameObject = Cast<UGASSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
	if (!SaveGameObject) return false;

	if (SaveGameObject->SavedGASActors.Remove(SavedActorID) > 0)
	{
		SaveGameObject->SaveTimestamp = FDateTime::Now();
		return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex);
	}

	return false;
}
