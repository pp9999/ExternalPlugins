#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <Structs.h>
#include <sol/sol.hpp> // Include Sol2

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif

using ItemType = std::variant<int, std::string>;

struct Inventory {
    struct InventoryItem
    {
        int id;
        std::string name;
        int amount;
        int slot;
        int xp = -1;
    };

    // Bools
    LIBRARY_API bool IsOpen();
    LIBRARY_API bool IsFull();
    LIBRARY_API bool IsEmpty();
    LIBRARY_API bool IsItemSelected();

    // Contains functions
    LIBRARY_API bool Contains(const std::variant<std::vector<ItemType>, sol::table>& items);
    LIBRARY_API bool ContainsAll(const std::variant<std::vector<ItemType>, sol::table>& items);
    LIBRARY_API bool ContainsAny(const std::variant<std::vector<ItemType>, sol::table>& items);
    LIBRARY_API bool ContainsOnly(const std::variant<std::vector<ItemType>, sol::table>& items);

    // Item amount and XP
    LIBRARY_API int GetItemAmount(const ItemType& item);
    LIBRARY_API int GetItemXp(const ItemType& item);

    // Actions
    LIBRARY_API bool Eat(const ItemType& item);
    LIBRARY_API bool Drop(const ItemType& item);
    LIBRARY_API bool Use(const ItemType& item);
    LIBRARY_API bool Equip(const ItemType& item);
    LIBRARY_API bool Rub(const ItemType& item);
    LIBRARY_API bool NoteItem(const ItemType& item);
    LIBRARY_API bool UseItemOnItem(const ItemType& item1, const ItemType& item2);
    LIBRARY_API bool DoAction(const ItemType& item, int action, int offset);

    // Inventory information
    LIBRARY_API int FreeSpaces();
    LIBRARY_API std::vector<InventoryItem> GetItem(const ItemType& item);
    LIBRARY_API std::vector<InventoryItem> GetItems();
    LIBRARY_API InventoryItem GetSlotData(int slot);

    static std::string RemoveTags(const std::string& input);

private:
    // Helper functions
    bool ContainsByID(int itemID);
    bool ContainsByName(const std::string& itemName);
    int GetItemAmountByID(int itemID);
    int GetItemAmountByName(const std::string& itemName);
    int GetItemXpByID(int itemID);
    int GetItemXpByName(const std::string& itemName);
    InventoryItem ConvertToInventoryItem(const IInfo& item);
};

#endif // INVENTORY_H