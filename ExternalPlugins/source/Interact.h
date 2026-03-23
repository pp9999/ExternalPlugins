#pragma once
#include <vector>
#include <string>
#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif



struct Interact {
    // Primary methods with optional parameters
    LIBRARY_API bool NPC(std::string name, std::string action, std::optional<WPOINT> coord = std::nullopt, std::optional<int> distance = std::nullopt);
    LIBRARY_API bool NPC(std::string name, std::string action, int distance); // Overload for name, action, distance

    LIBRARY_API bool Object(std::string name, std::string action, std::optional<WPOINT> coord = std::nullopt, std::optional<int> distance = std::nullopt);
    LIBRARY_API bool Object(std::string name, std::string action, int distance); // Overload for name, action, distance
    //LIBRARY_API bool Item();

    LIBRARY_API void SetSleep(int p1, int p2, int p3);
};
