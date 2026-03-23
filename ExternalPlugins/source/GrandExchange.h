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

// Forward declaration
struct GrandExchange;

// Order status enum for queue system
enum class OrderStatus {
    Pending = 0,
    Processing = 1,
    Completed = 2,
    Failed = 3
};

// Status return struct for Lua (must be before ProcessQueueConfig)
struct QueueStatus {
    int pending;
    int processing;
    int completed;
    int failed;
};

// Order struct for queue
struct GEOrder {
    int id;
    int type;  // 0 = BUY, 1 = SELL
    int itemId;
    std::string itemName;
    std::string priceStr;
    int quantity;
    OrderStatus status;
    std::string error;
    int retryCount;
    int maxRetries;
};

// ProcessQueue config builder
struct ProcessQueueConfig {
    int retries = 3;
    int retryDelayMs = 2000;
    int slotTimeoutMs = 30000;
    int collectTimeoutMs = 5000;

    ProcessQueueConfig& Retries(int r) { retries = r; return *this; }
    ProcessQueueConfig& RetryDelay(int ms) { retryDelayMs = ms; return *this; }
    ProcessQueueConfig& SlotTimeout(int ms) { slotTimeoutMs = ms; return *this; }
    ProcessQueueConfig& CollectTimeout(int ms) { collectTimeoutMs = ms; return *this; }
    QueueStatus Execute();  // Apply config and execute
};

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
        static constexpr int SELECTED_ITEM = 9465;
        static constexpr int INPUT_PRICE = 137;
        static constexpr int INPUT_SELL = 139;
        static constexpr int INPUT_QUANTITY = 136;
        static constexpr std::array<int, 8> SLOT_IDS = { 7, 28, 49, 70, 94, 118, 142, 166 };
    };

    // CS2 Component constants for script-based input
    struct CS2Components {
        static constexpr int QuantityInteract = 6881517;
        static constexpr int QuantityExpectedVarc = 6881514;
        static constexpr int QuantityInput = 6881515;
        static constexpr int PriceInteract = 6881558;
        static constexpr int PriceExpectedVarc = 6881555;
        static constexpr int PriceInput = 6881556;
    };

    // Price adjustment button components
    struct PriceButtons {
        static constexpr int Increase5 = 307;
        static constexpr int Increase20 = 314;
        static constexpr int Decrease5 = 294;
        static constexpr int Decrease20 = 287;
        static constexpr int Interface = 105;
    };

    static int delayOffset;
    static bool useCS2Scripts;

    // ========================================
    // Queue System (New API)
    // ========================================

    // Queue storage
    static std::vector<GEOrder> orderQueue;
    static int nextOrderId;
    static ProcessQueueConfig currentConfig;
    static GEOrder* processingOrder;

    // Queue methods
    LIBRARY_API int Queue(ORDER_TYPE type, const std::string& itemName, const std::string& priceStr, int quantity);
    LIBRARY_API int Queue(ORDER_TYPE type, int itemId, const std::string& priceStr, int quantity);
    LIBRARY_API int Queue(ORDER_TYPE type, const std::string& itemName, int price, int quantity);
    LIBRARY_API int Queue(ORDER_TYPE type, int itemId, int price, int quantity);
    LIBRARY_API ProcessQueueConfig ProcessQueue();
    LIBRARY_API QueueStatus Execute();
    LIBRARY_API bool IsProcessing();
    LIBRARY_API bool HasPending();
    LIBRARY_API void ClearQueue();

    // ========================================
    // Utility Methods (Keep)
    // ========================================

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
    LIBRARY_API int FindOrder(int itemId);
    LIBRARY_API bool CancelOrder(int slot);

    // ========================================
    // Internal Helpers
    // ========================================

    // CS2 script-based methods
    LIBRARY_API bool FocusInputField(int interactComponent, int expectedVarc, int timeoutMs = 2000);
    LIBRARY_API bool SendCS2Input(const std::string& inputComponent, const std::string& text);
    LIBRARY_API bool SetQuantityCS2(int quantity);
    LIBRARY_API bool SetPriceCS2(int price);
    LIBRARY_API bool SelectItemCS2(int itemId);

    // Percentage-based pricing
    LIBRARY_API bool IsPercentagePrice(const std::string& priceStr);
    LIBRARY_API int ParsePercentage(const std::string& percentStr);
    LIBRARY_API int CalculateButtonPresses(int targetPercent, int& presses20, int& presses5, bool& isIncrease);
    LIBRARY_API int CalculateButtonPressesFromChange(int changePercent, int& presses20, int& presses5, bool& isIncrease);
    LIBRARY_API bool PressPricePercentButtons(int targetPercent);
    LIBRARY_API bool SetPriceByPercent(const std::string& percentStr);

    // Quantity/Price (keep for internal use)
    LIBRARY_API bool SetQuantity(int quantity);
    LIBRARY_API bool SetPrice(int price);
    LIBRARY_API bool SelectItem(int itemId);
    LIBRARY_API bool ConfirmOrder();
    LIBRARY_API int SearchForItemInUI(int itemId);

    // Queue execution helpers
    LIBRARY_API bool ExecuteOrder(GEOrder& order);
    LIBRARY_API bool EnsureFreeSlot();
    LIBRARY_API QueueStatus GetQueueStatus();

    inline const char* ENUM_ToString(ORDER_TYPE type) {
        switch (type) {
        case ORDER_TYPE::BUY:    return "Buy";
        case ORDER_TYPE::SELL:   return "Sell";
        default:                return "Unknown";
        }
    }
}inline GEX;

#endif //GRAND_EXCHANGE_H
