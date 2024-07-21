#pragma once
#include <functional>
#include <vector>

class RenderEvent {
public:
    using EventCallback = std::function<void()>;

    //Single instance
    LIBRARY_API static RenderEvent& GetInstance() {
        static RenderEvent instance;
        return instance;
    }

    LIBRARY_API void Subscribe(const EventCallback& callback) {
        std::lock_guard<std::mutex> lock(mtx);
        eventHandlers.push_back(callback);
    }

    //Clear subscribers when script stopped
    LIBRARY_API void Clear() {
        std::lock_guard<std::mutex> lock(mtx);
        eventHandlers.clear();
    }

    LIBRARY_API void Update() {
        if (LoopyLoop == true && eventHandlers.size() > 0) {
            for (const auto& handler : eventHandlers) {
                handler();
            }
        }
    }

private:
    RenderEvent() {}
    std::vector<EventCallback> eventHandlers;
    std::mutex mtx;
};

LIBRARY_API RenderEvent renderEvent{};
