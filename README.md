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

## How to Use

### 1. Drop-in Actor Component (Zero-Code Setup)
Attach **`UGASSaveComponent`** to any Player Character, Companion, or Enemy Blueprint:
1. Set **`SavedActorID`**: A unique ID for the actor (e.g., `Player_01`).
2. Set **`DefaultSaveSlot`**: Save file name on disk (e.g., `Slot_1`).
3. Call **`Save GAS State`** or **`Load GAS State`** from Blueprints or C++.

### 2. Single-Node Blueprint Function Library
Call static nodes from any Blueprint:
* **`Save Actor GAS To Slot`**: Serializes Attributes, Tags, Abilities & Effects to disk.
* **`Load Actor GAS From Slot`**: Restores state onto the target actor from disk.
* **`Save Multiple Actors GAS To Slot`**: Batch saves an array of actors (Player + Party) at once.
* **`Load Multiple Actors GAS From Slot`**: Batch restores an array of actors at once.

### 3. Selective Saving & Filtering (`FGASSaveOptions` & `FGASRestoreOptions`)
To save/restore specific elements (e.g., Save Attributes & Tags, but skip Granted Abilities):
```cpp
// C++ Example: Skip saving Granted Abilities
FGASSaveOptions SaveOptions;
SaveOptions.bSaveGrantedAbilities = false; // ❌ Do not save GA

UGASSaveSystemLibrary::SaveActorGASToSlot(GetWorld(), MyPlayer, "Player_01", "Slot_1", 0, SaveOptions);
```
*(In Blueprints, expand **Save Options** or **Restore Options** in the Details Panel).*

---

## 📖 Complete Documentation
For full API reference, C++ examples, and slot management guide, see [**USAGE_GUIDE.md**](USAGE_GUIDE.md).

---

## License
MIT License. Free for commercial and open-source projects created by Anand M Rastapur.
