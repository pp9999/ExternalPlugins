#ifndef GRAND_EXCHANGE_H
#define GRAND_EXCHANGE_H

#include <string>
#include <vector>
#include <array>
#include <atomic>
#include <mutex>
#include <optional>

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

enum class GEOrderType {
    BUY,
    SELL
};

enum class GECollectionTarget {
    Inventory,
    Bank,
    Auto
};

// Status return struct for Lua (must be before ProcessQueueConfig)
struct QueueStatus {
    int pending = 0;
    int processing = 0;
    int completed = 0;
    int failed = 0;
};

// Order struct for queue
struct GEOrder {
    int id = 0;
    GEOrderType type = GEOrderType::BUY;
    int itemId = 0;
    std::string itemName;
    std::string priceStr;
    int quantity = 0;
    OrderStatus status = OrderStatus::Pending;
    std::string error;
    int retryCount = 0;
    int maxRetries = 0;
    int slot = -1;
    int expectedPrice = 0;
};

struct GEOrderSnapshot {
    int id = 0;
    GEOrderType type = GEOrderType::BUY;
    int itemId = 0;
    std::string itemName;
    std::string price;
    int quantity = 0;
    OrderStatus status = OrderStatus::Pending;
    std::string error;
    int attempts = 0;
    int slot = -1;
};

// ProcessQueue config builder
struct ProcessQueueConfig {
    int retries = 3;
    int retryDelayMs = 2000;
    int slotTimeoutMs = 30000;
    int collectTimeoutMs = 5000;

    GECollectionTarget collectionTarget = GECollectionTarget::Auto;

    ProcessQueueConfig& Retries(int r);
    ProcessQueueConfig& RetryDelay(int ms);
    ProcessQueueConfig& SlotTimeout(int ms);
    ProcessQueueConfig& CollectTimeout(int ms);
    ProcessQueueConfig& CollectionTarget(GECollectionTarget target);
    QueueStatus Execute();
};

struct GrandExchange {
    using ORDER_TYPE = GEOrderType;

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
        static constexpr int WINDOW_CUSTOM_PRICE_INPUT = 10;
        static constexpr int CUSTOM_PRICE_PERCENT_VARBIT = 16219;
        static constexpr int CUSTOM_PRICE_DECREASE_PERCENT_VARBIT = 16220;
        static constexpr int SELECTED_ITEM = 9465;
        static constexpr int INPUT_PRICE = 137;
        static constexpr int INPUT_SELL = 139;
        static constexpr int INPUT_QUANTITY = 136;
        static constexpr int SEARCH_SPRITE_OPEN = 27308;
        static constexpr int SEARCH_SPRITE_CLOSED = 27312;
        static constexpr int BACK_BUTTON = 201;
        static constexpr int CONFIRM_BUTTON = 212;
        static constexpr int MARKET_PRICE_BUTTON = 223;
        static constexpr int COLLECT_TO_BANK_BUTTON = 1;
        static constexpr int COLLECT_TO_INVENTORY_BUTTON = 6;
        static constexpr int SEARCH_RESULT_BUTTON = 229;
        static constexpr int CUSTOM_DECREASE_BUTTON = 189;
        static constexpr int CUSTOM_INCREASE_BUTTON = 193;
        static constexpr std::array<int, 8> BUY_SLOT_IDS = { 16, 30, 44, 58, 72, 86, 100, 114 };
        static constexpr std::array<int, 8> SELL_SLOT_IDS = { 19, 40, 61, 82, 106, 130, 154, 178 };
        static constexpr std::array<int, 8> SELECT_SLOT_IDS = { 12, 33, 54, 75, 99, 123, 147, 171 };
        static constexpr std::array<int, 8> CANCEL_SLOT_IDS = { 12, 26, 40, 54, 68, 82, 96, 110 };
    };

    // CS2 Component constants for script-based input
    struct CS2Components {
        static constexpr int QuantityInteract = 6881450;
        static constexpr int QuantityExpectedVarc = 6881447;
        static constexpr int QuantityInput = 6881448;
        static constexpr int PriceInteract = 6881465;
        static constexpr int PriceExpectedVarc = 6881462;
        static constexpr int PriceInput = 6881463;
        static constexpr int SearchInput = 6881613;
        static constexpr int QuantityValueVarc = 84;
        static constexpr int PriceValueVarc = 85;
        static constexpr int CustomDecreaseScript = 20882;
        static constexpr int CustomIncreaseScript = 20885;
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
    static std::mutex queueMutex;
    static std::atomic_bool processing;
    static std::atomic_int processingOrderId;

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
    LIBRARY_API void ClearFinishedOrders();
    LIBRARY_API std::vector<GEOrderSnapshot> GetQueuedOrders();
    LIBRARY_API std::optional<GEOrderSnapshot> GetQueuedOrder(int orderId);
    LIBRARY_API QueueStatus GetQueueStatus();

    // ========================================
    // Utility Methods (Keep)
    // ========================================

    LIBRARY_API std::vector<ExchangeEntry> GetData();
    LIBRARY_API ExchangeEntry GetSlotData(int slot);
    LIBRARY_API bool IsAtGE();
    LIBRARY_API bool IsGEWindowOpen();
    LIBRARY_API bool IsGESearchOpen();
    LIBRARY_API bool IsGEOfferEditorOpen();
    LIBRARY_API bool IsGEOverviewOpen();
    LIBRARY_API bool CollectToInventory();
    LIBRARY_API bool CollectToBank();
    LIBRARY_API bool CollectFinishedOffers(GECollectionTarget target, int timeoutMs = 5000);
    LIBRARY_API bool Back();
    LIBRARY_API bool Open();
    LIBRARY_API void DelayOffset(int offset);
    LIBRARY_API void Close();
    LIBRARY_API int GetAvailableSlots();
    LIBRARY_API int GetFinishedSlots();
    LIBRARY_API int GetNextAvailableSlot();
    LIBRARY_API bool OpenSlot(int slot);
    LIBRARY_API bool OpenSellSlot(int slot);
    LIBRARY_API bool SelectSlot(int slot);
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
    LIBRARY_API bool SetMarketPrice();
    LIBRARY_API bool SetCustomPricePercent(bool increase, int percent);

    // Quantity/Price (keep for internal use)
    LIBRARY_API bool SetQuantity(int quantity);
    LIBRARY_API bool SetPrice(int price);
    LIBRARY_API bool SelectItem(int itemId);
    LIBRARY_API bool ConfirmOrder();
    LIBRARY_API int SearchForItemInUI(int itemId);

    // Queue execution helpers
    LIBRARY_API bool ExecuteOrder(GEOrder& order);
    LIBRARY_API bool EnsureFreeSlot();
    LIBRARY_API bool WaitForOfferPlaced(const GEOrder& order, int slot, int timeoutMs);
    LIBRARY_API bool IsExpectedOfferPresent(const GEOrder& order, int slot = -1);
    LIBRARY_API bool EnsureGEOverview(int timeoutMs = 3000);
    LIBRARY_API bool SelectSellItemCS2(int itemId);
    inline const char* ENUM_ToString(ORDER_TYPE type) {
        switch (type) {
        case ORDER_TYPE::BUY:    return "Buy";
        case ORDER_TYPE::SELL:   return "Sell";
        default:                return "Unknown";
        }
    }
}inline GEX;

#endif //GRAND_EXCHANGE_H
