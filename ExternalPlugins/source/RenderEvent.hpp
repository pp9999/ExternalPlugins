#pragma once
#include "API.h"
#include <functional>
#include <mutex>
#include <vector>

class RenderEvent {
public:
    using EventCallback = std::function<void()>;

    //Single instance
    static RenderEvent& GetInstance() {
        static RenderEvent instance;
        return instance;
    }

    void Subscribe(const EventCallback& callback) {
        std::lock_guard<std::mutex> lock(mtx);
        eventHandlers.push_back(callback);
    }

    //Clear subscribers when script stopped
    void Clear() {
        std::lock_guard<std::mutex> lock(mtx);
        eventHandlers.clear();
    }

    void Update() {
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