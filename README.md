# GASSaveSystem for Unreal Engine 5

**Author:** Anand M Rastapur  
**License:** MIT License  

A lightweight, high-performance, production-ready C++ plugin for **Unreal Engine 5** designed to serialize and restore the state of the **Gameplay Ability System (GAS)**—including Attributes, Gameplay Tags, Granted Abilities, and Active Gameplay Effects.

---

## 📖 Full Usage & Setup Guide
For detailed documentation on saving & restoring **Gameplay Attributes**, **Gameplay Tags**, **Gameplay Abilities (GA)**, and **Gameplay Effects (GE)**, see the [**USAGE_GUIDE.md**](USAGE_GUIDE.md).

---

## Features
- **Attribute Serialization:** Saves base and current values for all attributes in spawned `UAttributeSet` instances.
- **Gameplay Tag Preservation:** Retains active container states (`FGameplayTagContainer`).
- **Ability Restoration:** Re-grants abilities with level and input binding IDs intact.
- **Active Effect Recovery:** Captures stack counts, duration, and level for active `UGameplayEffect` buffs/debuffs.
- **Drop-in Actor Component:** `UGASSaveComponent` provides zero-code setup for saving player and enemy states.
- **Blueprint Function Library:** Static nodes accessible from Blueprints or C++.

---

## Installation & Setup
1. Place this plugin inside your project's `Plugins/` folder (`YourProject/Plugins/GASSaveSystem`).
2. Ensure the `GameplayAbilities` plugin is enabled in your `.uproject` file.
3. Right-click your `.uproject` file and select **Generate Visual Studio project files**.
4. Open your project solution and build in Visual Studio / Rider.

---

## Usage Guide

### 1. Drop-in Component (Blueprint & C++)
Attach `UGASSaveComponent` to your Character or Enemy Actor:
* Set `SavedActorID` to a unique identifier (e.g. `Player_0` or `Boss_Dragon`).
* Call `SaveGASState()` or `LoadGASState()` from Blueprints or C++.

### 2. Static Function Library (Blueprint Nodes)
Use the `UGASSaveSystemLibrary` static nodes in Blueprints:
* **`SaveActorGASToSlot`**: Saves an Actor's entire GAS state to disk.
* **`LoadActorGASFromSlot`**: Restores an Actor's GAS state from a save file.

### 3. C++ Direct API
```cpp
#include "GASSaveSystemLibrary.h"

// Save GAS State to Struct
FGASActorSaveData SaveData;
UGASSaveSystemLibrary::SaveAbilitySystemComponent(MyAbilitySystemComponent, SaveData);

// Restore GAS State from Struct
UGASSaveSystemLibrary::RestoreAbilitySystemComponent(MyAbilitySystemComponent, SaveData);
```

---

## License
MIT License. Free for commercial and open-source projects.
