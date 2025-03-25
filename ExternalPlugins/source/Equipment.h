#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include <vector>
#include <array>
#include <Structs.h>
#include <variant>
#include <sol/sol.hpp> // Include Sol2

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif

using ItemType = std::variant<int, std::string>;

struct Equipment
{
    struct EquipmentItem
    {
        int id;
        std::string name;
        int slot;
        int amount;
        int xp = -1;
    };

    enum class ESlot
    {
        HEAD = 0,
        CAPE = 1,
        NECK = 2,
        MAINHAND = 3,
        BODY = 4,
        OFFHAND = 5,
        BOTTOM = 7,
        GLOVES = 9,
        BOOTS = 10,
        RING = 12,
        AMMO = 13,
        AURA = 14,
        POCKET = 17
    };

    // Bools
    LIBRARY_API bool IsOpen();
    LIBRARY_API bool OpenInterface();
    LIBRARY_API bool IsEmpty();
    LIBRARY_API bool IsFull();

    // Contains methods
    LIBRARY_API bool Contains(const std::variant<ItemType, sol::table>& itemVariant);
    LIBRARY_API bool ContainsAll(const std::variant<std::vector<ItemType>, sol::table>& itemsVariant);
    LIBRARY_API bool ContainsAny(const std::variant<std::vector<ItemType>, sol::table>& itemsVariant);
    LIBRARY_API bool ContainsOnly(const std::variant<std::vector<ItemType>, sol::table>& itemsVariant);

    // Actions
    LIBRARY_API bool Unequip(const std::variant<int, std::string, sol::table>& itemVariant);

    // Get item(s)
    LIBRARY_API int GetItemID(ESlot slot);
    LIBRARY_API int GetItemXp(ESlot slot);
    LIBRARY_API EquipmentItem GetSlotData(ESlot slot);
    LIBRARY_API std::vector<EquipmentItem> GetItems();

    // Get gear
    LIBRARY_API EquipmentItem GetHelm();
    LIBRARY_API EquipmentItem GetCape();
    LIBRARY_API EquipmentItem GetNeck();
    LIBRARY_API EquipmentItem GetMainhand();
    LIBRARY_API EquipmentItem GetBody();
    LIBRARY_API EquipmentItem GetOffhand();
    LIBRARY_API EquipmentItem GetBottom();
    LIBRARY_API EquipmentItem GetGloves();
    LIBRARY_API EquipmentItem GetBoots();
    LIBRARY_API EquipmentItem GetRing();
    LIBRARY_API EquipmentItem GetAmmo();
    LIBRARY_API EquipmentItem GetAura();
    LIBRARY_API EquipmentItem GetPocket();

private:
    // Helper functions
    bool ContainsByID(int itemID);
    bool ContainsByName(const std::string& itemName);
    int GetItemXpByID(int itemID);
    EquipmentItem ConvertToEquipmentItem(const IInfo& item);
    static std::string RemoveTags(const std::string& input);
};

#endif EQUIPMENT_H