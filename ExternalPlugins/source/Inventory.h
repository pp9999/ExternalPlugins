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
    LIBRARY_API bool Contains(const sol::object& item);
    LIBRARY_API bool ContainsAll(const sol::table& luaTable);
    LIBRARY_API bool ContainsAny(const sol::table& luaTable);
    LIBRARY_API bool ContainsOnly(const sol::table& luaTable);

    // Item amount and XP
    LIBRARY_API int GetItemAmount(const sol::object& item);
    LIBRARY_API int GetItemXp(const sol::object& item);

    // Actions
    LIBRARY_API bool Eat(const sol::object& item);
    LIBRARY_API bool Drop(const sol::object& item);
    LIBRARY_API bool Use(const sol::object& item);
    LIBRARY_API bool Equip(const sol::object& item);
    LIBRARY_API bool Rub(const sol::object& item);
    LIBRARY_API bool NoteItem(const sol::object& item);
    LIBRARY_API bool UseItemOnItem(const sol::object& item1, const sol::object& item2);
    LIBRARY_API bool DoAction(const sol::object& item, int action, int offset);

    // Inventory information
    LIBRARY_API int FreeSpaces();
    LIBRARY_API std::vector<InventoryItem> GetItem(const sol::object& item);
    LIBRARY_API std::vector<InventoryItem> GetItems();
    LIBRARY_API InventoryItem GetSlotData(int slot);

private:
    // Helper functions
    bool ContainsByID(int itemID);
    bool ContainsByName(const std::string& itemName);
    int GetItemAmountByID(int itemID);
    int GetItemAmountByName(const std::string& itemName);
    int GetItemXpByID(int itemID);
    int GetItemXpByName(const std::string& itemName);
    InventoryItem ConvertToInventoryItem(const IInfo& item);
    static std::string RemoveTags(const std::string& input);
};

#endif INVENTORY_H
