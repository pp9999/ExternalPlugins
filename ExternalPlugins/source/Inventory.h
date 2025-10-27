#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <vector>
#include <variant>
#include <API.h>
#include <sol/sol.hpp>

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

    //combine containers and cache
    LIBRARY_API std::vector<inv_Container_struct> ReadInvArrays33(bool GetCoords = false);

    // random functions
    LIBRARY_API bool IsOpen();
    LIBRARY_API bool IsFull();
    LIBRARY_API int Invfreecount();
    LIBRARY_API bool IsEmpty();
    LIBRARY_API bool IsItemSelected();
    LIBRARY_API int InvItemcount(int item);
    LIBRARY_API bool InvItemFound(int item);
    LIBRARY_API bool InvItemFounds(std::vector<int> items);
    LIBRARY_API int InvItemcount_String(std::string item);
    LIBRARY_API uint64_t InvItemcountStack_Strings(std::string item);
    LIBRARY_API std::vector<int> InvItemcounts(std::vector<int> item);
    LIBRARY_API uint64_t InvStackSize(int item);
    LIBRARY_API bool NoteStuff(int item);
    LIBRARY_API bool CheckInvStuffCheckAll(std::vector<int> items);
    LIBRARY_API bool CheckInvStuffCheckAllSS(std::vector<int> items, int size, bool sizeorstack);

    // Contains functions
    LIBRARY_API bool Contains(const std::variant<int, std::string, std::vector<ItemType>, sol::table>& itemsVariant);
    LIBRARY_API bool ContainsAll(const std::variant<std::vector<ItemType>, sol::table>& items);
    LIBRARY_API bool ContainsAny(const std::variant<std::vector<ItemType>, sol::table>& items);
    LIBRARY_API bool ContainsOnly(const std::variant<std::vector<ItemType>, sol::table>& items);

    // Item amount and XP
    LIBRARY_API uint64_t GetItemAmount(const ItemType& item);
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
    uint64_t GetItemAmountByID(int itemID);
    int GetItemAmountByName(const std::string& itemName);
    int GetItemXpByID(int itemID);
    int GetItemXpByName(const std::string& itemName);
    InventoryItem ConvertToInventoryItem(const IInfo& item);
}inline InventoryClass;

#endif // INVENTORY_H
