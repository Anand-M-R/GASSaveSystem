# GASSaveSystem - Complete Setup & Usage Guide

**Author:** Anand M Rastapur  
**Target Engine:** Unreal Engine 5.0+  
**License:** MIT License  

---

## 📋 Table of Contents
1. [Overview](#overview)
2. [Plugin Installation & Setup](#plugin-installation--setup)
3. [Core Capabilities](#core-capabilities)
   - [1. Gameplay Attributes](#1-gameplay-attributes)
   - [2. Gameplay Tags (FGameplayTagContainer)](#2-gameplay-tags-fgameplaytagcontainer)
   - [3. Gameplay Abilities (GA)](#3-gameplay-abilities-ga)
   - [4. Gameplay Effects (GE - Buffs/Debuffs)](#4-gameplay-effects-ge---buffsdebuffs)
4. [Blueprint Usage Guide](#blueprint-usage-guide)
5. [C++ Usage Guide & API Reference](#c-usage-guide--api-reference)
6. [Multi-Actor & Save Slot Management](#multi-actor--save-slot-management)
7. [Troubleshooting & FAQ](#troubleshooting--faq)

---

## Overview
`GASSaveSystem` is a production-ready C++ plugin designed to solve the persistence gap in Epic Games' **Gameplay Ability System (GAS)**. Because GAS was originally designed for session-based multiplayer games (*Paragon*, *Fortnite*), it does not natively provide a save/load mechanism. 

`GASSaveSystem` automatically extracts, serializes, and restores full GAS state across single-player RPGs, checkpoints, and level transitions.

---

## Plugin Installation & Setup

### Step 1: Copy Plugin
Copy the `GASSaveSystem` folder into your project's `Plugins/` directory:
```text
YourUnrealProject/
└── Plugins/
    └── GASSaveSystem/
        ├── GASSaveSystem.uplugin
        ├── Source/
        └── USAGE_GUIDE.md
```

### Step 2: Enable Plugin in `.uproject`
Ensure `GameplayAbilities` and `GASSaveSystem` are enabled in your project's `.uproject` file:
```json
"Plugins": [
    {
        "Name": "GameplayAbilities",
        "Enabled": true
    },
    {
        "Name": "GASSaveSystem",
        "Enabled": true
    }
]
```

### Step 3: Add Module Dependency (C++ Projects)
If writing C++ code, add `"GASSaveSystem"` to your project's `Build.cs` file:
```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "GameplayAbilities",
    "GameplayTags",
    "GASSaveSystem"
});
```

---

## Core Capabilities

### 1. Gameplay Attributes
`GASSaveSystem` dynamically inspects every `UAttributeSet` attached to your `UAbilitySystemComponent` (ASC) via C++ reflection.

* **What gets saved:**
  * `AttributeName` (e.g. `Health`, `MaxHealth`, `Mana`, `Stamina`, `Strength`, `Armor`)
  * `BaseValue` (Permanent stat value without temporary modifiers)
  * `CurrentValue` (Stat value after active modifiers/buffs)
* **How it restores:**
  Upon loading, `SetNumericAttributeBase()` and `ApplyModToAttribute(Override)` are executed on each attribute, triggering native attribute change delegates (e.g., `PreAttributeChange` and `PostGameplayEffectExecute`).

---

### 2. Gameplay Tags (`FGameplayTagContainer`)
Gameplay Tags represent status flags, combat states, and world triggers.

* **What gets saved:**
  * Active container tags e.g., `State.Poisoned`, `State.Stunned`, `Buff.Shielded`, `Quest.BossDefeated`.
* **How it restores:**
  The saved `FGameplayTagContainer` is assigned back to the character's `AbilitySystemComponent`, ensuring all status tags persist.

---

### 3. Gameplay Abilities (GA)
Granted abilities represent unlocked spells, attacks, or movement skills.

* **What gets saved:**
  * `AbilityClass`: The `TSubclassOf<UGameplayAbility>` asset (e.g., `GA_Fireball_C`, `GA_DoubleJump_C`).
  * `AbilityLevel`: Integer level of the spell (e.g., Level 3 Fireball).
  * `InputID`: Hotkey assignment integer index.
* **How it restores:**
  Iterates through saved ability specs. If an ability is not currently granted, it calls `ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, Level, InputID))` automatically.

---

### 4. Gameplay Effects (GE - Buffs/Debuffs)
Active Gameplay Effects represent ongoing temporary or stacking status effects.

* **What gets saved:**
  * `EffectClass`: The `TSubclassOf<UGameplayEffect>` asset (e.g., `GE_PoisonOverTime_C`, `GE_HasteBuff_C`).
  * `Duration` & `TimeRemaining`: Active duration timers.
  * `StackCount`: Current stack multiplier (e.g., 3x Poison stacks).
  * `EffectLevel`: Level of the applied effect.
* **How it restores:**
  Creates outgoing effect specs via `ASC->MakeOutgoingSpec()` and applies them to self with exact stack counts and remaining durations restored.

---

## Blueprint Usage Guide

### Method A: Drop-in `UGASSaveComponent` (Zero-Code Setup)
1. Open your Player or Enemy Blueprint (e.g. `BP_PlayerCharacter`).
2. Click **+ Add Component** -> Search for **`GASSaveComponent`**.
3. In the **Details** panel:
   * **`SavedActorID`**: Set a unique ID (e.g. `Player_01` or `Boss_Dragon`).
   * **`DefaultSaveSlot`**: Set slot file name (e.g. `Slot_1`).
   * **`bAutoLoadOnBeginPlay`**: Set `true` to auto-restore on map load.

4. **Saving in Blueprint:**
   Call **`Save GAS State`** on the component.

5. **Loading in Blueprint:**
   Call **`Load GAS State`** on the component.

---

### Method B: Blueprint Function Library Static Nodes
You can also use static nodes anywhere in your Blueprints without attaching a component:

* **`Save Actor GAS To Slot`**:
  * `TargetActor`: Actor reference.
  * `SavedActorID`: `"Player_01"`.
  * `SlotName`: `"SaveSlot_1"`.
* **`Load Actor GAS From Slot`**:
  * `TargetActor`: Actor reference.
  * `SavedActorID`: `"Player_01"`.
  * `SlotName`: `"SaveSlot_1"`.

---

### Selective Saving & Filtering (`FGASSaveOptions` & `FGASRestoreOptions`)
If you only want to save/restore specific elements (e.g. Save Attributes & Tags, but ignore Granted Abilities):

* **`FGASSaveOptions`**:
  * `bSaveAttributes` (default `true`)
  * `bSaveGameplayTags` (default `true`)
  * `bSaveGrantedAbilities` (default `true` — set `false` to skip saving abilities)
  * `bSaveActiveEffects` (default `true`)

* **`FGASRestoreOptions`**:
  * `bRestoreAttributes` (default `true`)
  * `bRestoreGameplayTags` (default `true`)
  * `bRestoreGrantedAbilities` (default `true`)
  * `bRestoreActiveEffects` (default `true`)

**In `UGASSaveComponent` Details Panel:**
Expand **Save Options** or **Restore Options** in the Inspector to uncheck any element you do not wish to save or restore.

---

## C++ Usage Guide & API Reference

### Including Headers
```cpp
#include "GASSaveSystemLibrary.h"
#include "GASSaveComponent.h"
#include "GASSaveTypes.h"
```

### Saving GAS State to Struct (In-Memory)
```cpp
FGASActorSaveData SaveData;
SaveData.SavedActorID = TEXT("Player_01");

// Extract attributes, tags, abilities, and effects from ASC
bool bSuccess = UGASSaveSystemLibrary::SaveAbilitySystemComponent(MyAbilitySystemComponent, SaveData);
```

### Restoring GAS State from Struct
```cpp
// Re-apply attributes, tags, abilities, and effects onto ASC
bool bSuccess = UGASSaveSystemLibrary::RestoreAbilitySystemComponent(MyAbilitySystemComponent, SaveData);
```

### Direct Disk Save & Load API
```cpp
// Save directly to disk file: Saved/SaveGames/SaveSlot_1.sav
UGASSaveSystemLibrary::SaveActorGASToSlot(GetWorld(), MyCharacter, TEXT("Player_01"), TEXT("SaveSlot_1"));

// Load directly from disk file: Saved/SaveGames/SaveSlot_1.sav
UGASSaveSystemLibrary::LoadActorGASFromSlot(GetWorld(), MyCharacter, TEXT("Player_01"), TEXT("SaveSlot_1"));
```

---

## Multi-Actor & Save Slot Management

A single save file (`.sav`) can store multiple actors at once!

```cpp
// Save Player 1
UGASSaveSystemLibrary::SaveActorGASToSlot(GetWorld(), PlayerActor, TEXT("Player_01"), TEXT("CheckpointSlot"));

// Save Companion / Party Member
UGASSaveSystemLibrary::SaveActorGASToSlot(GetWorld(), CompanionActor, TEXT("Companion_01"), TEXT("CheckpointSlot"));

// Save Boss Enemy State
UGASSaveSystemLibrary::SaveActorGASToSlot(GetWorld(), BossActor, TEXT("Boss_Dragon"), TEXT("CheckpointSlot"));
```

All three actors will be stored inside `Saved/SaveGames/CheckpointSlot.sav`.

---

## Troubleshooting & FAQ

#### Q: Where are save files stored?
> Save files are stored in your Unreal Engine project directory at:
> `YourProject/Saved/SaveGames/<SlotName>.sav`

#### Q: Why is my Health UI bar not updating when loading?
> Ensure your UI widget reads `AttributeSet->GetHealth() / AttributeSet->GetMaxHealth()` and that `MaxHealth` is set to `100.0f` rather than `5.0f`.

#### Q: How do I handle new abilities added in game updates?
> `RestoreAbilitySystemComponent` safely checks if an ability class is already granted before calling `GiveAbility()`, preventing duplicate ability specs.
