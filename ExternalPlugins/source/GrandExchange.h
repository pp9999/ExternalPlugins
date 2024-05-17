#include <string>
#include <vector>
#ifndef GRAND_EXCHANGE_H
#define GRAND_EXCHANGE_H

#include <string>
#include <vector>
#include <array>

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif

struct GrandExchange {
    enum class ORDER_TYPE {
        BUY,
        SELL
    };

    struct ExchangeEntry {
        int status;
        ORDER_TYPE orderType;
        int item_id;
        int price;
        int quantity;
        int completed_quantity;
        int completed_value;
    };

    // Define constants
    struct Constants {
        static constexpr int GRAND_EXCHANGE_AREA[] = { 3164, 3477 };
        static constexpr int MAX_ENTRIES = 8;
        static constexpr int ENTRY_SIZE = 0x28;
        static constexpr int WINDOW_GE_OPEN = 82;
        static constexpr int WINDOW_CHAT_OPEN = 11;
        static constexpr int SELECTED_ITEM = 9465;//135 was doing some stuff on legacy
        static constexpr int INPUT_PRICE = 137;
        static constexpr int INPUT_SELL = 139;
        static constexpr int INPUT_QUANTITY = 136;
        static constexpr std::array<int, 8> SLOT_IDS = { 7, 28, 49, 70, 94, 118, 142, 166 };
    };

    static int delayOffset;

    // Function declarations
    LIBRARY_API std::vector<ExchangeEntry> GetData();
    LIBRARY_API ExchangeEntry GetSlotData(int slot);
    LIBRARY_API bool IsAtGE();
    LIBRARY_API bool IsGEWindowOpen();
    LIBRARY_API bool IsGESearchOpen();
    LIBRARY_API bool CollectToInventory();
    LIBRARY_API bool Back();
    LIBRARY_API bool Open();
    LIBRARY_API void DelayOffset(int offset);
    LIBRARY_API void Close();
    LIBRARY_API int GetAvailableSlots();
    LIBRARY_API int GetFinishedSlots();
    LIBRARY_API int GetNextAvailableSlot();
    LIBRARY_API bool OpenSlot(int slot);
    LIBRARY_API bool OpenNextAvailableSlot();
    LIBRARY_API bool SetQuantity(int quantity);
    LIBRARY_API bool SetPrice(int price);
    LIBRARY_API int SearchForItemInUI(int itemId);
    LIBRARY_API bool SelectItem(int itemId);
    LIBRARY_API bool ConfirmOrder();
    LIBRARY_API int FindOrder(int itemId);
    LIBRARY_API bool CancelOrder(int slot);
    LIBRARY_API bool PlaceOrder(ORDER_TYPE type, int itemId, const std::string& itemName, int price, int quantity);
};

#endif // GRAND_EXCHANGE_H